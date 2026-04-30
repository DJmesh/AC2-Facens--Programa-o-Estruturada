from rest_framework import viewsets
from .models.task import TaskBoard, TaskColumn, TaskItem
from .serializers import TaskBoardSerializer, TaskColumnSerializer, TaskItemSerializer

class TaskBoardViewSet(viewsets.ModelViewSet):
    queryset = TaskBoard.objects.all()
    serializer_class = TaskBoardSerializer

class TaskColumnViewSet(viewsets.ModelViewSet):
    queryset = TaskColumn.objects.all()
    serializer_class = TaskColumnSerializer

class TaskItemViewSet(viewsets.ModelViewSet):
    queryset = TaskItem.objects.all()
    serializer_class = TaskItemSerializer
