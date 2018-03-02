#pragma once

#include "cinder/gl/gl.h"
#include "Box2D/Box2D.h"

namespace cinder { namespace gl {

class b2DubugDraw : public b2Draw {

public:

	class PolygonBatch {

	public:

		PolygonBatch(GLenum draw_type);
		void add(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
		void draw();

	private:

		uint32_t mSize;
		uint32_t mNumVertices;
		uint32_t mStrideSize;
		GLenum mDrawType;
		ci::gl::VaoRef mVao;
		ci::gl::GlslProgRef mGlsl;
		ci::gl::VboRef mData;

	};

	class CircleLineBatch {

	public:

		CircleLineBatch();
		void add(const b2Vec2& center, float32 radius, const b2Color& color);
		void draw();

	private:

		uint32_t mSize;
		uint32_t mNumInstances;
		uint32_t mInstanceStride;
		ci::gl::VboRef mData;
		gl::BatchRef mBatch;

	};

	class CircleFillBatch {

	public:

		CircleFillBatch();
		void add(const b2Vec2& center, float32 radius, const b2Color& color);
		void draw();

	private:

		uint32_t mSize;
		uint32_t mNumInstances;
		uint32_t mInstanceStride;
		ci::gl::VboRef mData;
		gl::BatchRef mBatch;

	};

	class SegmentBatch {

	public:

		SegmentBatch();
		void add(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
		void draw();

	private:

		uint32_t mSize;
		uint32_t mNumVertices;
		uint32_t mStrideSize;
		ci::gl::VaoRef mVao;
		ci::gl::VboRef mData;
		ci::gl::GlslProgRef mGlsl;

	};

	/// Draw a closed polygon provided in CCW order.
	virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)override;

	/// Draw a solid closed polygon provided in CCW order.
	virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)override;

	/// Draw a circle.
	virtual void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) override;

	/// Draw a solid circle.
	virtual void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)override;

	/// Draw a line segment.
	virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)override;

	/// Draw a transform. Choose your own length scale.
	/// @param xf a transform.
	virtual void DrawTransform(const b2Transform& xf)override;

	void draw();

private:
	 
	CircleLineBatch mCircleLineBatch;
	CircleFillBatch mCircleFillBatch;
	PolygonBatch mPolygonLineBatch{ GL_LINE_LOOP };
	PolygonBatch mPolygonFillBatch{ GL_TRIANGLES };
	SegmentBatch mSegmentBatch;

};
	
} }