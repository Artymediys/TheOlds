# Generated by Django 2.0.3 on 2018-05-27 05:59

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('main', '0009_profile_bot_token'),
    ]

    operations = [
        migrations.AlterField(
            model_name='profile',
            name='tm_sesison',
            field=models.CharField(max_length=40, null=True),
        ),
    ]