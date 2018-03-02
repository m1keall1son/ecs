from __future__ import unicode_literals

from django.db import models

# Create your models here.
class LogMessage(models.Model):
	MESSAGE_TYPE = (
	('0', 'DEBUG'),
	('1', 'LOG'), 
	('2', 'ANALYTICS')
	)
	
	MessageType = models.CharField(max_length = 1, choices=MESSAGE_TYPE, default='0')
	Message = models.TextField()
	DateTime = models.DateTimeField()
	
	def __str__(self):
		return self.DateTime.strftime("%I:%M %p %Y %B %d")