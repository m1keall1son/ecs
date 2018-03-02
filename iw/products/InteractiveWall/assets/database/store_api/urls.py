from django.conf.urls import url
from store_api import views

urlpatterns = [
    url(r'^categories/$', views.category_list),
    url(r'^category/(?P<pk>[0-9]+)/$', views.category_detail),
    url(r'^subcategories/$', views.subcategory_list),
    url(r'^subcategory/(?P<pk>[0-9]+)/$', views.subcategory_detail),
	url(r'^clouds/$', views.cloud_list),
    url(r'^cloud/(?P<pk>[0-9]+)/$', views.cloud_detail),
	url(r'^products/$', views.product_list),
    url(r'^product/(?P<pk>[0-9\-]+)/$', views.product_detail),
	url(r'^allergens/$', views.allergen_list),
    url(r'^allergen/(?P<pk>[\w-]+)/$', views.allergen_detail),
	url(r'^parameters/$', views.parameter_list),
    url(r'^parameter/(?P<pk>[0-9]+)/$', views.parameter_detail)
]