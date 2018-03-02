#include "ciDebugDraw.h"
#include "cinder/Log.h"

#include "cinder/app/App.h"

using namespace ci;

void ci::gl::b2DubugDraw::DrawPolygon(const b2Vec2 * vertices, int32 vertexCount, const b2Color & color)
{
	gl::ScopedColor scoped_color(Color(color.r, color.g, color.b));
	auto vert_batch = ci::gl::VertBatch::create();

	vert_batch->begin(GL_LINE_LOOP);

	for (int i = 0; i < vertexCount; i++ ) {
		vert_batch->vertex( vertices[i].x, vertices[i].y );
	}

	vert_batch->end();

	vert_batch->draw();
}

void ci::gl::b2DubugDraw::DrawSolidPolygon(const b2Vec2 * vertices, int32 vertexCount, const b2Color & color)
{

	gl::ScopedColor scoped_color(ColorA(color.r, color.g, color.b, .5));
	auto vert_batch = ci::gl::VertBatch::create();

	vec2 avg_center = vec2(0);
	for (int i = 0; i < vertexCount; i++) {
		avg_center += vec2(vertices[i].x, vertices[i].y);
	}

	avg_center /= vec2(vertexCount);

	vert_batch->begin(GL_TRIANGLE_FAN);

	for (int i = 0; i < vertexCount-1; i++) {
		vert_batch->vertex(avg_center);
		vert_batch->vertex(vertices[i].x, vertices[i].y);
		vert_batch->vertex(vertices[i + 1].x, vertices[i + 1].y);
	}

	vert_batch->vertex(avg_center);
	vert_batch->vertex(vertices[vertexCount-1].x, vertices[vertexCount-1].y);
	vert_batch->vertex(vertices[0].x, vertices[0].y);

	vert_batch->end();

	vert_batch->draw();

	DrawPolygon(vertices, vertexCount, color);

}

void ci::gl::b2DubugDraw::DrawCircle(const b2Vec2 & center, float32 radius, const b2Color & color)
{
	mCircleLineBatch.add(center, radius, color);
}

void ci::gl::b2DubugDraw::DrawSolidCircle(const b2Vec2 & center, float32 radius, const b2Vec2 & axis, const b2Color & color)
{
	mCircleLineBatch.add(center, radius, color);
	mCircleFillBatch.add(center, radius, color);

}

void ci::gl::b2DubugDraw::DrawSegment(const b2Vec2 & p1, const b2Vec2 & p2, const b2Color & color)
{
	gl::ScopedColor scoped_color(Color(color.r, color.g, color.b));
	gl::drawLine(vec2(p1.x, p1.y), vec2(p2.x, p2.y));
}

void ci::gl::b2DubugDraw::DrawTransform(const b2Transform & xf)
{
	gl::ScopedColor scoped_color(Color(.5, .5, .5));
	gl::drawStrokedCircle(vec2(xf.p.x, xf.p.y), 5, 12);
	gl::ScopedColor scoped_axis(Color(1., 0., 0.));
	gl::drawLine(vec2(xf.p.x, xf.p.y), vec2(xf.p.x, xf.p.y) + vec2(xf.q.c * 5, xf.q.s * 5));
}

void cinder::gl::b2DubugDraw::draw()
{
	mCircleFillBatch.draw();
	mCircleLineBatch.draw();
}

cinder::gl::b2DubugDraw::PolygonBatch::PolygonBatch(GLenum draw_type):mDrawType(draw_type)
{

}

void cinder::gl::b2DubugDraw::PolygonBatch::add(const b2Vec2 * vertices, int32 vertexCount, const b2Color & color)
{
}

void cinder::gl::b2DubugDraw::PolygonBatch::draw()
{
}


cinder::gl::b2DubugDraw::SegmentBatch::SegmentBatch()
{
}

void cinder::gl::b2DubugDraw::SegmentBatch::add(const b2Vec2 & p1, const b2Vec2 & p2, const b2Color & color)
{
}

void cinder::gl::b2DubugDraw::SegmentBatch::draw()
{
}

static std::string circle_vert = CI_GLSL(150,

in vec4 ciPosition;
in vec2 center;
in float radius;
in vec4 color;

out vec4 vColor;

uniform mat4 ciModelViewProjection;

void main() {
	vColor = color;
	gl_Position = ciModelViewProjection * vec4(ciPosition.x*radius + center.x, ciPosition.y*radius + center.y, 0., 1.);
}

);

static std::string circle_frag = CI_GLSL(150,

out vec4 FragColor;

in vec4 vColor;

void main() {
	FragColor = vColor;
}

);

cinder::gl::b2DubugDraw::CircleLineBatch::CircleLineBatch() : mNumInstances(0)
{
	mData = gl::Vbo::create( GL_ARRAY_BUFFER, sizeof(vec4)*1000, nullptr, GL_DYNAMIC_DRAW );
	 
	auto mesh = gl::VboMesh::create( geom::WireCircle().subdivisions(64) );

	auto buffer_layout = geom::BufferLayout();
	buffer_layout.append( geom::Attrib::CUSTOM_0, 1, sizeof(float) * 7, 0, 1 );
	buffer_layout.append( geom::Attrib::CUSTOM_1, 2, sizeof(float) * 7, sizeof(float), 1);
	buffer_layout.append( geom::Attrib::CUSTOM_3, 4, sizeof(float) * 7, sizeof(float)*3, 1);

	mInstanceStride = buffer_layout.calcRequiredStorage(1);

	mesh->appendVbo( buffer_layout, mData );

	gl::GlslProgRef glsl;

	try {
		glsl = gl::GlslProg::create( gl::GlslProg::Format().vertex( circle_vert ).fragment( circle_frag ).attrib(geom::Attrib::CUSTOM_0, "radius").attrib(geom::CUSTOM_1, "center").attrib(geom::CUSTOM_3, "color"));
	}
	catch (const ci::gl::GlslProgCompileExc& e)
	{
		CI_LOG_E(e.what());
	}

	mBatch = gl::Batch::create( mesh, glsl );

}

void cinder::gl::b2DubugDraw::CircleLineBatch::add(const b2Vec2 & center, float32 radius, const b2Color & color)
{
	auto mapped = mData->mapBufferRange(mNumInstances*mInstanceStride, mInstanceStride, GL_MAP_WRITE_BIT );

	auto mapped_radius = (float*)mapped;
	*mapped_radius++ = radius;
	*mapped_radius++ = center.x;
	*mapped_radius++ = center.y;
	*mapped_radius++ = color.r;
	*mapped_radius++ = color.g;
	*mapped_radius++ = color.b;
	*mapped_radius++ = .75f;
	mData->unmap();

	mNumInstances++;
}

void cinder::gl::b2DubugDraw::CircleLineBatch::draw()
{
	mBatch->drawInstanced(mNumInstances);
	mNumInstances = 0;
}

cinder::gl::b2DubugDraw::CircleFillBatch::CircleFillBatch() : mNumInstances(0)
{
	mData = gl::Vbo::create(GL_ARRAY_BUFFER, sizeof(vec4) * 1000, nullptr, GL_DYNAMIC_DRAW);

	auto mesh = gl::VboMesh::create(geom::Circle().subdivisions(64));

	auto buffer_layout = geom::BufferLayout();
	buffer_layout.append(geom::Attrib::CUSTOM_0, 1, sizeof(float) * 7, 0, 1);
	buffer_layout.append(geom::Attrib::CUSTOM_1, 2, sizeof(float) * 7, sizeof(float), 1);
	buffer_layout.append(geom::Attrib::CUSTOM_3, 4, sizeof(float) * 7, sizeof(float) * 3, 1);

	mInstanceStride = buffer_layout.calcRequiredStorage(1);

	mesh->appendVbo(buffer_layout, mData);

	gl::GlslProgRef glsl;

	try {
		glsl = gl::GlslProg::create(gl::GlslProg::Format().vertex(circle_vert).fragment(circle_frag).attrib(geom::Attrib::CUSTOM_0, "radius").attrib(geom::CUSTOM_1, "center").attrib(geom::CUSTOM_3, "color"));
	}
	catch (const ci::gl::GlslProgCompileExc& e)
	{
		CI_LOG_E(e.what());
	}

	mBatch = gl::Batch::create(mesh, glsl);
}

void cinder::gl::b2DubugDraw::CircleFillBatch::add(const b2Vec2 & center, float32 radius, const b2Color & color)
{
	auto mapped = mData->mapBufferRange(mNumInstances*mInstanceStride, mInstanceStride, GL_MAP_WRITE_BIT);

	auto mapped_radius = (float*)mapped;
	*mapped_radius++ = radius;
	*mapped_radius++ = center.x;
	*mapped_radius++ = center.y;
	*mapped_radius++ = color.r;
	*mapped_radius++ = color.g;
	*mapped_radius++ = color.b;
	*mapped_radius++ = .5f;
	mData->unmap();

	mNumInstances++;
}

void cinder::gl::b2DubugDraw::CircleFillBatch::add(const ci::vec2 & center, float32 radius, const ci::ColorA & color)
{
	auto mapped = mData->mapBufferRange(mNumInstances*mInstanceStride, mInstanceStride, GL_MAP_WRITE_BIT);

	auto mapped_radius = (float*)mapped;
	*mapped_radius++ = radius;
	*mapped_radius++ = center.x;
	*mapped_radius++ = center.y;
	*mapped_radius++ = color.r;
	*mapped_radius++ = color.g;
	*mapped_radius++ = color.b;
	*mapped_radius++ = color.a;
	mData->unmap();

	mNumInstances++;
}

void cinder::gl::b2DubugDraw::CircleFillBatch::draw()
{
	mBatch->drawInstanced(mNumInstances);
	mNumInstances = 0;
}
