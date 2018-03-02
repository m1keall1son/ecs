from django.contrib import admin
from .models import *

# Register your models here.
class CategoryAdmin(admin.ModelAdmin):
    fields = ['type', 'uuid', 'name', 'description', 'promotion', 'far_away', 'inventory', 'background', 'icon', 'etag']

class SubCategoryAdmin(admin.ModelAdmin):
	fields = ['type', 'uuid', 'name', 'description', 'promotion', 'priority', 'icon', 'clouds']

class CloudAdmin(admin.ModelAdmin):
 	fields = ['type', 'uuid', 'name', 'description', 'promotion', 'icon', 'products', 'etag']
	
class ProductAdmin(admin.ModelAdmin):
    fields = ['type', 'name', 'brand', 'description', 'promotion', 'barcode', 'sku', 'price', 
		'in_stock', 'display_size', 'sale_start_time', 'sale_end_time', 'sale_price',
		'sale_description', 'allergens', 'image', 'thumbnail', 'etag']

class AllergenAdmin(admin.ModelAdmin):
	fields = ['type', 'name', 'description']

class GlobalParameterAdmin(admin.ModelAdmin):
	fields = ['type', 'key', 'value', 'datatype']
    
admin.site.register(Category, CategoryAdmin)
admin.site.register(SubCategory, SubCategoryAdmin)
admin.site.register(Cloud, CloudAdmin)
admin.site.register(Product, ProductAdmin)
admin.site.register(Allergen, AllergenAdmin)
admin.site.register(GlobalParameter, GlobalParameterAdmin)