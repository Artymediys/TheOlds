cd /home/btm/btm
echo "Stop gunicorn server"
sudo /usr/sbin/service gunicorn_btm stop

echo "Get code"
git checkout master
git pull
git checkout production
git merge master

echo "Upgrade packages"
/home/btm/btm/venv/bin/pip install -r /home/btm/btm/requirements.txt

echo "Refresh db"
/home/btm/btm/venv/bin/python manage.py migrate

echo "Some cheats with static files"
rm -rf /home/btm/btm/static_root
/home/btm/btm/venv/bin/python manage.py collectstatic
mv /home/btm/btm/static_root /home/btm/btm/static_tmp
mkdir /home/btm/btm/static_root
mv /home/btm/btm/static_tmp/ /home/btm/btm/static_root/static

echo "make documentation"
rm -rf /home/btm/btm/docs
doxygen

echo "restart server"
sudo /usr/sbin/service gunicorn_btm start
sudo /usr/sbin/service nginx restart

