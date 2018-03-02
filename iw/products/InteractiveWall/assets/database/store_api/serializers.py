from rest_framework import serializers
from store_api.models import *

class CategorySerializer(serializers.ModelSerializer):
    class Meta:
        model = Category
        fields = ('type', 'uuid', 'name', 'description', 'promotion', 'far_away', 'background', 'icon', 
		'inventory', 'etag')
		
class SubCategorySerializer(serializers.ModelSerializer):
	class Meta:
		model = SubCategory
		fields = ('type', 'uuid', 'name', 'description', 'promotion', 'icon', 'clouds')
		
class CloudSerializer(serializers.ModelSerializer):
	class Meta:
		model = Cloud
		fields = ('type', 'uuid', 'name', 'description', 'promotion', 'icon', 'products', 'etag')
		
class ProductSerializer(serializers.ModelSerializer):
	class Meta:
		model = Product
		fields = ('type', 'name', 'brand', 'description', 'promotion', 'barcode', 'sku', 'price', 
		'in_stock', 'display_size', 'promotion', 'sale_start_time', 'sale_end_time', 'sale_price',
		'sale_description', 'allergens', 'image', 'thumbnail', 'etag')
		
class AllergenSerializer(serializers.ModelSerializer):
	class Meta:
		model = Allergen
		fields = ('type', 'name', 'description')
		
class GlobalParameterSerializer(serializers.ModelSerializer):
	class Meta:
		model = GlobalParameter
		fields = ('type', 'key', 'value', 'datatype')