from django.shortcuts import render
from django.http import HttpResponse
from django.views.decorators.csrf import csrf_exempt
from rest_framework.renderers import JSONRenderer
from rest_framework.parsers import JSONParser
from store_api.models import *
from store_api.serializers import *

# Create your views here.
class JSONResponse(HttpResponse):
    """
    An HttpResponse that renders its content into a JSON object
    """
    def __init__(self, data, **kwargs):
        content = JSONRenderer().render(data)
        kwargs['content_type'] = 'application/json'
        super(JSONResponse, self).__init__(content, **kwargs)
        
        
@csrf_exempt
def category_list(request):
    """
    List all categories, or create a new category.
    """
    if request.method == 'GET':
        categories = Category.objects.all()
        serializer = CategorySerializer(categories, many=True)
        return JSONResponse(serializer.data)

    elif request.method == 'POST':
        data = JSONParser().parse(request)
        serializer = CategorySerializer(data=data)
        if serializer.is_valid():
            serializer.save()
            return JSONResponse(serializer.data, status=201)
        return JSONResponse(serializer.errors, status=400)
        
@csrf_exempt
def category_detail(request, pk):
    """
    Retrieve, update or delete a category.
    """
    try:
        category = Category.objects.get(pk=pk)
    except Category.DoesNotExist:
        return HttpResponse(status=404)

    if request.method == 'GET':
        serializer = CategorySerializer(category)
        return JSONResponse(serializer.data)

    elif request.method == 'PUT':
        data = JSONParser().parse(request)
        serializer = CategorySerializer(category, data=data)
        if serializer.is_valid():
            serializer.save()
            return JSONResponse(serializer.data)
        return JSONResponse(serializer.errors, status=400)

    elif request.method == 'DELETE':
        category.delete()
        return HttpResponse(status=204)
		
@csrf_exempt
def subcategory_list(request):
    """
    List all subcategories, or create a new subcategory.
    """
    if request.method == 'GET':
        subcategories = SubCategory.objects.all()
        serializer = SubCategorySerializer(subcategories, many=True)
        return JSONResponse(serializer.data)

    elif request.method == 'POST':
        data = JSONParser().parse(request)
        serializer = SubCategorySerializer(data=data)
        if serializer.is_valid():
            serializer.save()
            return JSONResponse(serializer.data, status=201)
        return JSONResponse(serializer.errors, status=400)
        
@csrf_exempt
def subcategory_detail(request, pk):
    """
    Retrieve, update or delete a subcategory.
    """
    try:
        subcategory = SubCategory.objects.get(pk=pk)
    except SubCategory.DoesNotExist:
        return HttpResponse(status=404)

    if request.method == 'GET':
        serializer = SubCategorySerializer(subcategory)
        return JSONResponse(serializer.data)

    elif request.method == 'PUT':
        data = JSONParser().parse(request)
        serializer = SubCategorySerializer(subcategory, data=data)
        if serializer.is_valid():
            serializer.save()
            return JSONResponse(serializer.data)
        return JSONResponse(serializer.errors, status=400)

    elif request.method == 'DELETE':
        subcategory.delete()
        return HttpResponse(status=204)
		
@csrf_exempt
def cloud_list(request):
    """
    List all clouds, or create a new cloud.
    """
    if request.method == 'GET':
        categories = Cloud.objects.all()
        serializer = CloudSerializer(categories, many=True)
        return JSONResponse(serializer.data)

    elif request.method == 'POST':
        data = JSONParser().parse(request)
        serializer = CloudSerializer(data=data)
        if serializer.is_valid():
            serializer.save()
            return JSONResponse(serializer.data, status=201)
        return JSONResponse(serializer.errors, status=400)
        
@csrf_exempt
def cloud_detail(request, pk):
    """
    Retrieve, update or delete a cloud.
    """
    try:
        cloud = Cloud.objects.get(pk=pk)
    except Cloud.DoesNotExist:
        return HttpResponse(status=404)

    if request.method == 'GET':
        serializer = CloudSerializer(cloud)
        return JSONResponse(serializer.data)

    elif request.method == 'PUT':
        data = JSONParser().parse(request)
        serializer = CloudSerializer(cloud, data=data)
        if serializer.is_valid():
            serializer.save()
            return JSONResponse(serializer.data)
        return JSONResponse(serializer.errors, status=400)

    elif request.method == 'DELETE':
        cloud.delete()
        return HttpResponse(status=204)	
		
@csrf_exempt
def product_list(request):
    """
    List all categories, or create a new product.
    """
    if request.method == 'GET':
        categories = Product.objects.all()
        serializer = ProductSerializer(categories, many=True)
        return JSONResponse(serializer.data)

    elif request.method == 'POST':
        data = JSONParser().parse(request)
        serializer = ProductSerializer(data=data)
        if serializer.is_valid():
            serializer.save()
            return JSONResponse(serializer.data, status=201)
        return JSONResponse(serializer.errors, status=400)
        
@csrf_exempt
def product_detail(request, pk):
    """
    Retrieve, update or delete a product.
    """
    try:
        product = Product.objects.get(pk=pk)
    except Product.DoesNotExist:
        return HttpResponse(status=404)

    if request.method == 'GET':
        serializer = ProductSerializer(product)
        return JSONResponse(serializer.data)

    elif request.method == 'PUT':
        data = JSONParser().parse(request)
        serializer = ProductSerializer(product, data=data)
        if serializer.is_valid():
            serializer.save()
            return JSONResponse(serializer.data)
        return JSONResponse(serializer.errors, status=400)

    elif request.method == 'DELETE':
        product.delete()
        return HttpResponse(status=204)
		
@csrf_exempt
def allergen_list(request):
    """
    List all categories, or create a new allergen.
    """
    if request.method == 'GET':
        categories = Allergen.objects.all()
        serializer = AllergenSerializer(categories, many=True)
        return JSONResponse(serializer.data)

    elif request.method == 'POST':
        data = JSONParser().parse(request)
        serializer = AllergenSerializer(data=data)
        if serializer.is_valid():
            serializer.save()
            return JSONResponse(serializer.data, status=201)
        return JSONResponse(serializer.errors, status=400)
        
@csrf_exempt
def allergen_detail(request, pk):
    """
    Retrieve, update or delete a allergen.
    """
    try:
        allergen = Allergen.objects.get(pk=pk)
    except Allergen.DoesNotExist:
        return HttpResponse(status=404)

    if request.method == 'GET':
        serializer = AllergenSerializer(allergen)
        return JSONResponse(serializer.data)

    elif request.method == 'PUT':
        data = JSONParser().parse(request)
        serializer = AllergenSerializer(allergen, data=data)
        if serializer.is_valid():
            serializer.save()
            return JSONResponse(serializer.data)
        return JSONResponse(serializer.errors, status=400)

    elif request.method == 'DELETE':
        allergen.delete()
        return HttpResponse(status=204)
		
@csrf_exempt
def parameter_list(request):
    """
    List all categories, or create a new parameter.
    """
    if request.method == 'GET':
        categories = GlobalParameter.objects.all()
        serializer = GlobalParameterSerializer(categories, many=True)
        return JSONResponse(serializer.data)

    elif request.method == 'POST':
        data = JSONParser().parse(request)
        serializer = GlobalParameterSerializer(data=data)
        if serializer.is_valid():
            serializer.save()
            return JSONResponse(serializer.data, status=201)
        return JSONResponse(serializer.errors, status=400)
        
@csrf_exempt
def parameter_detail(request, pk):
    """
    Retrieve, update or delete a parameter.
    """
    try:
        parameter = GlobalParameter.objects.get(pk=pk)
    except GlobalParameter.DoesNotExist:
        return HttpResponse(status=404)

    if request.method == 'GET':
        serializer = GlobalParameterSerializer(parameter)
        return JSONResponse(serializer.data)

    elif request.method == 'PUT':
        data = JSONParser().parse(request)
        serializer = GlobalParameterSerializer(parameter, data=data)
        if serializer.is_valid():
            serializer.save()
            return JSONResponse(serializer.data)
        return JSONResponse(serializer.errors, status=400)

    elif request.method == 'DELETE':
        parameter.delete()
        return HttpResponse(status=204)	