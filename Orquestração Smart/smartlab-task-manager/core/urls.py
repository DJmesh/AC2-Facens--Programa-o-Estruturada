from django.contrib import admin
from django.urls import path, include
from rest_framework import permissions
from drf_yasg.views import get_schema_view
from drf_yasg import openapi
from django.contrib.auth.decorators import login_required

schema_view = get_schema_view(
   openapi.Info(
      title="SmartTask API",
      default_version='v1',
      description="API para gestão de tarefas profissional",
   ),
   public=False, # Protegido
   permission_classes=(permissions.IsAdminUser,),
)

urlpatterns = [
    path('admin/', admin.site.name),
    path('api/tasks/', include('tasks.urls')),
    
    # Docs protegidos por login do admin
    path('swagger/', login_required(schema_view.with_ui('swagger', cache_timeout=0)), name='schema-swagger-ui'),
    path('redoc/', login_required(schema_view.with_ui('redoc', cache_timeout=0)), name='schema-redoc'),
]
