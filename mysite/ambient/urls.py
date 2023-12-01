from django.urls import path
from . import views

urlpatterns = [
    path('addData/', views.addData),
    path('line_chart/', views.line_chart, name='line_chart'),
]