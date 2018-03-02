from django.shortcuts import render
from .models import LogMessage

# Create your views here.	
def index(request):
    log_list = LogMessage.objects.order_by('-DateTime')[:10]
    context = {'log_list': log_list}
    return render(request, 'logs/index.html', context)
	
#def detail(request, question_id):
#    question = get_object_or_404(Question, pk=question_id)
#    return render(request, 'polls/detail.html', {'question': question})