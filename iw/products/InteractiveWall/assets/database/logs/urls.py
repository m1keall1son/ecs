from django.conf.urls import url
from . import views

app_name = 'logs'

urlpatterns = [
    # ex: /polls/
    url(r'^$', views.index, name='index'),
	]