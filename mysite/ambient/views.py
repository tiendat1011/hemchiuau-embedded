from django.shortcuts import render
from rest_framework.decorators import api_view
from .models import dht22
from .serializers import ItemSerializer
from rest_framework.response import Response
from django.db.models import Count
from datetime import datetime
from django.db.models.functions import TruncSecond
from django.db.models import F
from django.utils import timezone
import pytz
import json

#Create your views here.
@api_view(['POST'])
def addData(request):
    record_count = dht22.objects.aggregate(count=Count('id'))['count']
    LIMIT = 10000
    if record_count < LIMIT:
        serializer = ItemSerializer(data = request.data)
        if serializer.is_valid():
            serializer.save()
        return Response(serializer.data)
    else:
        return Response("Ló Chim bùm")



def line_chart(request):
    data = dht22.objects.annotate(date=TruncSecond('timestamp')).values('date').annotate(temperature=F('temperature'), humidity=F('humidity')).order_by('date')[:60]
    for entry in data:
        entry['date'] = entry['date'].astimezone(pytz.timezone('Asia/Ho_Chi_Minh'))
    timestamps = [entry['date'].strftime('%Y-%m-%dT%H:%M:%S.%fZ') for entry in data]
    #timestamps = [entry['date'].astimezone(timezone('Asia/Ho_Chi_Minh')) for entry in data]
    temperatures = [entry['temperature'] for entry in data]
    humidities = [entry['humidity'] for entry in data]

    return render(request, 'line_chart.html', {'timestamps': json.dumps(timestamps), 'temperatures': temperatures, 'humidities': humidities})
# class SensorDataView(APIView):
#     parser_classes = [JSONParser]

#     def post(self, request, *args, **kwargs):
#         serializer = ItemSerializer(data=request.data)

#         if serializer.is_valid():
#             serializer.save()
#             return Response("Data received and saved.", status=status.HTTP_200_OK)
#         else:
#             return Response(serializer.errors, status=status.HTTP_400_BAD_REQUEST)