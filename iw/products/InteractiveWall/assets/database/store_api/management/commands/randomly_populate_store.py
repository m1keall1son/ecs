from django.core.management.base import BaseCommand
from django.utils import timezone
from store_api.models import *
import os, json, random, datetime

class Command(BaseCommand):
    args = '<>'
    help = 'Populates database with random data.'
	
    def __populate_product__(self, filename, json_field):
        skus = []
        
        with open(os.path.abspath(filename)) as data_file:
            product_data = json.load(data_file)[json_field]
			
        num_products = random.randint(12, 25)
            
        for i in range(num_products):
            new_product = Product(type=3, 
								name=random.choice(product_data), 
								brand="", 
								description="", 
								promotion="", 
								barcode="", 
								sku=random.randrange(1024), 
								price=round(random.uniform(0.99, 10.00), 2), 
								in_stock=True, 
								display_size=0, 
								sale_start_time=timezone.now(), 
								sale_end_time=timezone.now(), 
								sale_price=round(random.uniform(0.99, 3.00), 2),
								sale_description="", 
								allergens=["None"], 
								image="images/test_pattern.png", 
								thumbnail="images/test_pattern.png", 
								etag=0xDEADBEEF)
            skus.append(new_product.sku)
            new_product.save()
			
        return skus

    def __new_cloud__(self, products):
        id = random.randint(0,1024)
        
        new_cloud = Cloud(type=2,
                    uuid=id,
                    name="Cloud %s" % id,
                    description="",
                    promotion="",
                    icon="images/test_pattern.png",
                    products=products,
                    etag=0xDEADD00D
                    )
        new_cloud.save()
                    
        return new_cloud.uuid
        
    def __create_clouds__(self, sku_list):
        length = len(sku_list)
        first = random.randint(0, int(length/2.0))
        second = random.randint(int(length/2.0), length)

        cloud_0 = self.__new_cloud__(sku_list[0:first])
        cloud_1 = self.__new_cloud__(sku_list[first:second])
        cloud_2 = self.__new_cloud__(sku_list[second:])

        clouds = [cloud_0, cloud_1, cloud_2]
        
        return clouds
        
    def __create_subcategory__(self, clouds):
        id = random.randint(0,1024)
        
        subcategory = SubCategory(type=1,
                                uuid=id,
                                name="SubCategory %s" % id,
                                description="",
                                priority=1,
                                promotion="",
                                icon="images/test_pattern.png",
                                clouds=clouds
                                )
        subcategory.save()
        return subcategory.uuid
        
    def __create_category__(self, subcategory):
        id = random.randint(0,64)
        
        category = Category(type=0,
                            uuid=id,
                            name="Category %s" % id,
                            description="",
                            promotion="",
                            far_away="images/test_pattern.png",
                            background="images/test_pattern.png",
                            icon="images/test_pattern.png",
                            inventory=[subcategory],
                            etag=0xBEEFD00D
                            )
        category.save()
        return category.name
        
    def __populate_database__(self):
        #print "Running database population..."
		
        allergen = Allergen(name="None", description="none")
        allergen.save()
        
        fruit_skus = self.__populate_product__("store_api/data/fruits.json", "fruits")		
        vegetable_skus = self.__populate_product__("store_api/data/vegetables.json", "vegetables")
        bread_skus = self.__populate_product__("store_api/data/breads_and_pastries.json", "breads")

        skus = [fruit_skus, vegetable_skus, bread_skus]
        
        for sku_list in skus:
            cloud_ids = self.__create_clouds__(sku_list)
            subcategory_id = self.__create_subcategory__(cloud_ids)
            category_id = self.__create_category__(subcategory_id)
            
    def handle(self, *args, **options):
        self.__populate_database__()