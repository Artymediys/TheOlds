Проект "Billy the messenger"
=================================

[![pipeline status](https://gitlab.informatics.ru/smirnov/billy_the_messenger/badges/develop/pipeline.svg)](https://gitlab.informatics.ru/smirnov/billy_the_messenger/commits/develop)

Технологический стек
---------------------------------
- python 3
- django
- postgresql

Подготовка к работе
---------------------------------

- Установка всех необходимых библиотек:
---------------------------------
```bash
pip install -r requirements.txt
sudo apt-get install postgresql
sudo apt-get install postgresql-server-dev-9.5
```
Также, для работы websockets вам понадобится redis (проект можно запустить и без них)

В большинстве дистрибутивов он не входит в стандартные репозитории

Инструкция для [archlinux](https://wiki.archlinux.org/index.php/Redis), [ubuntu](https://www.digitalocean.com/community/tutorials/how-to-install-and-configure-redis-on-ubuntu-16-04)

По умолчанию redis принимает подключения со всех адресов и это можно изменить в ```/etc/redis/redis.conf``` или в ```/etc/redis.conf``` (в зависимости от дистрибутива)

```bash
# /etc/redis.conf
bind 127.0.0.1
port 6379
```

- Настройка БД
---------------------------------
```bash
sudo -u postgres psql postgres
```

```sql
create user mtb_user with password 'mtbpass';
alter role mtb_user set client_encoding to 'utf8';
alter role mtb_user set default_transaction_isolation to 'read committed';
alter role mtb_user set timezone to 'UTC';
ALTER USER mtb_user CREATEDB;
create database mtb_db owner mtb_user;
\q
```

```bash
python manage.py migrate
```

-Запуск сервера
---------------------------------
```bash
python manage.py runserver
```