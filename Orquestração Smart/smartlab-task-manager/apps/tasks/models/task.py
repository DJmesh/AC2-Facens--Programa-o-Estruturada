from django.db import models
from apps.base.models.base import SmartBaseModel

class TaskBoard(SmartBaseModel):
    name = models.CharField(max_length=255)
    description = models.TextField(blank=True, null=True)

    def __str__(self):
        return self.name

class TaskColumn(SmartBaseModel):
    board = models.ForeignKey(TaskBoard, related_name='columns', on_delete=models.CASCADE)
    title = models.CharField(max_length=100)
    order = models.PositiveIntegerField(default=0)

    class Meta(SmartBaseModel.Meta):
        ordering = ['order']

    def __str__(self):
        return f"{self.board.name} - {self.title}"

class TaskItem(SmartBaseModel):
    PRIORITY_CHOICES = [
        ('LOW', 'Low'),
        ('MEDIUM', 'Medium'),
        ('HIGH', 'High'),
        ('CRITICAL', 'Critical'),
    ]
    
    column = models.ForeignKey(TaskColumn, related_name='tasks', on_delete=models.CASCADE)
    title = models.CharField(max_length=255)
    description = models.TextField(blank=True, null=True)
    priority = models.CharField(max_length=10, choices=PRIORITY_CHOICES, default='MEDIUM')
    order = models.PositiveIntegerField(default=0)
    due_date = models.DateTimeField(null=True, blank=True)

    class Meta(SmartBaseModel.Meta):
        ordering = ['order']

    def __str__(self):
        return self.title
