import uuid
from django.db import models

class SmartBaseModel(models.Model):
    """
    Base model for all entities in the SmartLab ecosystem.
    Uses UUID as primary key for scalability and security.
    """
    id = models.UUIDField(primary_key=True, default=uuid.uuid4, editable=False)
    created_at = models.DateTimeField(auto_now_add=True)
    updated_at = models.DateTimeField(auto_now=True)
    is_active = models.BooleanField(default=True)

    class Meta:
        abstract = True
        ordering = ['-created_at']
