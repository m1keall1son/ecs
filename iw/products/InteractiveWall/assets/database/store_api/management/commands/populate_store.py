from django.core.management.base import BaseCommand
from django.utils import timezone
from store_api.models import *
import os, json, random, datetime

class Command(BaseCommand):
    args = '<>'
    help = 'Populates database with json data.'
    
    def __display_size__(self):
        seed = random.random()
        if seed < 0.08 :
            return 2
        elif seed < 0.16:
            return 1
        else:
            return 0
	
    def __populate_product__(self, filename, json_field):
        products = []
        
        with open(os.path.abspath(filename)) as data_file:
            product_data = json.load(data_file)[json_field]
			
        for i in range(len(product_data)):
            #print product_data[i]
            new_product = Product(type=3, 
								name=product_data[i]["name"], 
								brand=product_data[i]["brand"], 
								description="",
								promotion=product_data[i]["promotion"], 
								barcode="", 
								sku=product_data[i]["sku"],
								price=product_data[i]["price"], 
								in_stock=True, 
								display_size=self.__display_size__(),
								sale_start_time=timezone.now(), 
								sale_end_time=timezone.now(), 
								sale_price=round(random.uniform(0.99, 3.00), 2),
								sale_description="", 
								allergens=["None"], 
								image="%s.png" % product_data[i]["sku"],
								thumbnail="%s.png" % product_data[i]["sku"], 
								etag=0xDEADBEEF)
            products.append(new_product)
            new_product.save()
			
        return products

    def __new_cloud__(self, uuid, name, skus):
        id = random.randint(0,1024)
        
        new_cloud = Cloud(type=2,
                    uuid=uuid,
                    name=name,
                    description="",
                    promotion="",
                    icon="images/test_pattern.png",
                    products=skus,
                    etag=0xDEADD00D
                    )
        new_cloud.save()
                    
        return new_cloud.uuid
        
    def __create_clouds__(self, filename, json_field):
        cloud_dict = {}
        cloud_name_dict = {}
        clouds = []
        
        with open(os.path.abspath(filename)) as data_file:
            product_data = json.load(data_file)[json_field]
	
        for i in range(len(product_data)):
            cloud_id = product_data[i]["cloud id"] - 1

            if cloud_id not in cloud_dict:
                cloud_dict[cloud_id] = []
                cloud_name_dict[cloud_id] = product_data[i]["brand"]

            cloud_dict[cloud_id].append(product_data[i]["sku"])

        for i in range(len(cloud_name_dict)):
            clouds.append(self.__new_cloud__(i, cloud_name_dict[i], cloud_dict[i]))
            
        return clouds
        
    def __create_subcategory__(self, clouds):
        
        subcategory = SubCategory(type=1,
                                uuid=24,
                                name="Ice Cream",
                                description="",
                                priority=1,
                                promotion="",
                                icon="images/test_pattern.png",
                                clouds=clouds
                                )
        subcategory.save()
        return subcategory.uuid
        
    def __create_category__(self, subcategory):        
        category = Category(type=0,
                            uuid=17,
                            name="Frozen Desserts",
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
        
        desserts = self.__populate_product__("store_api/data/desserts.json", "products")
        cloud_ids = self.__create_clouds__("store_api/data/desserts.json", "products")
        subcategory_id = self.__create_subcategory__(cloud_ids)
        self.__create_category__(subcategory_id)
            
    def handle(self, *args, **options):
        self.__populate_database__()