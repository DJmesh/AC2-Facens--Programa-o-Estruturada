from rest_framework import serializers
from .models.task import TaskBoard, TaskColumn, TaskItem

class TaskItemSerializer(serializers.ModelSerializer):
    class Meta:
        model = TaskItem
        fields = '__all__'

class TaskColumnSerializer(serializers.ModelSerializer):
    tasks = TaskItemSerializer(many=True, read_only=True)
    class Meta:
        model = TaskColumn
        fields = '__all__'

class TaskBoardSerializer(serializers.ModelSerializer):
    columns = TaskColumnSerializer(many=True, read_only=True)
    class Meta:
        model = TaskBoard
        fields = '__all__'
