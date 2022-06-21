from django.contrib.auth.models import User
from django.test import TestCase, Client
from django.urls import reverse
from main.forms import SignUpForm, VkAuthForm
from django.contrib.auth.forms import AuthenticationForm
from main.tm import send_message
from pyrogram.api import errors
from pyrogram import Client as tmClient


class TestSignInPage(TestCase):

    def setUp(self):
        u = User(username="testuser", first_name="Vasya", last_name="Pupkin", is_active=1)
        u.set_password('pass')
        u.save()
        self.client = Client()
        self.url = reverse("signin")

    def test_signin_page_downloaded(self):
        response = self.client.get(self.url)
        self.assertEqual(response.status_code, 200)

    def test_signin_page_title(self):
        response = self.client.get(self.url)
        self.assertEqual(response.context['title'], 'Billy the Messenger')

    def test_signin_page_template(self):
        response = self.client.get(self.url)
        self.assertTemplateUsed(response, 'billy_the_messenger/signin.html')

    def test_signin_page_wrong_data(self):
        response = self.client.post(self.url, {'username': 'testuser', 'password': 'passh'})
        f = AuthenticationForm(data = response.wsgi_request.POST)
        f.is_valid()
        array = f.errors['__all__']
        flag = False
        for i in range(len(array)):
            if array[i] == 'Пожалуйста, введите правильные имя пользователя и пароль. ' \
                           'Оба поля могут быть чувствительны к регистру.':
                flag = True
        self.assertTrue(flag)

    def test_signin_page_required_password(self):
        response = self.client.post(self.url, {'username': 'testuser', 'passwor': 'passh'})
        f = AuthenticationForm(data = response.wsgi_request.POST)
        f.is_valid()
        array = f.errors['password']
        flag = False
        for i in range(len(array)):
            if array[i] == 'Обязательное поле.':
                flag = True
        self.assertTrue(flag)

    def test_signin_page_required_username(self):
        response = self.client.post(self.url, {'usernam': 'testuse', 'password': 'pass'})
        f = AuthenticationForm(data = response.wsgi_request.POST)
        f.is_valid()
        array = f.errors['username']
        flag = False
        for i in range(len(array)):
            if array[i] == 'Обязательное поле.':
                flag = True
        self.assertTrue(flag)

    def test_signin_page_empty_fields(self):
        response1 = self.client.post(self.url, {'username': 'testing_user', 'password': ''})
        response2 = self.client.post(self.url, {'username': '', 'password': 'pass'})
        f1 = AuthenticationForm(data=response1.wsgi_request.POST)
        f2 = AuthenticationForm(data=response2.wsgi_request.POST)
        f1.is_valid()
        f2.is_valid()
        array1 = f1.errors['password']
        array2 = f2.errors['username']
        flag1 = False
        for i in range(len(array1)):
            if array1[i] == 'Обязательное поле.':
                flag1 = True
        flag2 = False
        for i in range(len(array2)):
            if array2[i] == 'Обязательное поле.':
                flag2 = True
        self.assertTrue(flag1)
        self.assertTrue(flag2)

    def test_signin_page_ok(self):
        response = self.client.post(self.url, {'username': 'testuser', 'password': 'pass'})
        f = AuthenticationForm(data=response.wsgi_request.POST)
        f.is_valid()
        dict = f.errors
        flag = False
        if len(dict) == 0:
            flag = True
        self.assertTrue(flag)


class TestSignUpPage(TestCase):

    def setUp(self):
        self.client = Client()
        self.url = reverse("signup")

    def test_signup_page_downloaded(self):
        response = self.client.get(self.url)
        self.assertEqual(response.status_code, 200)

    def test_signup_page_title(self):
        response = self.client.get(self.url)
        self.assertEqual(response.context['title'], 'Billy the Messenger')

    def test_signup_page_template(self):
        response = self.client.get(self.url)
        self.assertTemplateUsed(response, 'billy_the_messenger/signup.html')

    def test_signup_page_help_text(self):
        form = SignUpForm()
        self.assertEqual(form.fields['email'].help_text, 'Required. Inform a valid email address.')
        self.assertEqual(form.fields['username'].help_text,
                         'Обязательное поле. Не более 150 символов. Только буквы, цифры и символы @/./+/-/_.')
        self.assertEqual(form.fields['password1'].help_text,
                         '<ul><li>Ваш пароль не должен совпадать с вашим именем или другой персональной информацией '
                         'или быть слишком похожим на неё.</li><li>Ваш пароль должен содержать как минимум 8 символов.'
                         '</li><li>Ваш пароль не может быть одним из широко распространённых паролей.</li><li>'
                         'Ваш пароль не может состоять только из цифр.</li></ul>')
        self.assertEqual(form.fields['password2'].help_text, 'Для подтверждения введите, пожалуйста, пароль ещё раз.')

    def test_signup_page_short_password(self):
        response = self.client.post(self.url, {'username': 'testing_user', 'email': 'test@mail.ru',
                                               'password1': 'pasS5!', 'password2': 'pasS5!'})
        f = SignUpForm(response.wsgi_request.POST)
        f.is_valid()
        array = f.errors['password2']
        flag = False
        for i in range(len(array)):
            if array[i] == 'Введённый пароль слишком короткий. Он должен содержать как минимум 8 символов.':
                flag = True
        self.assertTrue(flag)

    def test_signup_page_like_email_and_username_password(self):
        response1 = self.client.post(self.url, {'username': 'viva_user', 'email': 'testingtest@mail.ru',
                                               'password1': 'testingtest', 'password2': 'testingtest'})
        response2 = self.client.post(self.url, {'username': 'testing_user', 'email': 'testingtest@mail.ru',
                                               'password1': 'testing_user', 'password2': 'testing_user'})
        f1 = SignUpForm(response1.wsgi_request.POST)
        f2 = SignUpForm(response2.wsgi_request.POST)
        f1.is_valid()
        f2.is_valid()
        array1 = f1.errors['password2']
        array2 = f2.errors['password2']
        flag1 = False
        flag2 = False
        for i in range(len(array1)):
            if array1[i] == 'Введённый пароль слишком похож на адрес электронной почты.':
                flag1 = True
        for i in range(len(array2)):
            if array2[i] == 'Введённый пароль слишком похож на имя пользователя.':
                flag2 = True
        self.assertTrue(flag1)
        self.assertTrue(flag2)

    def test_signup_page_numeral_password(self):
        response = self.client.post(self.url, {'username': 'testing_user', 'email': 'testingtest@mail.ru',
                                               'password1': '123456789', 'password2': '123456789'})
        f = SignUpForm(response.wsgi_request.POST)
        f.is_valid()
        array = f.errors['password2']
        flag = False
        for i in range(len(array)):
            if array[i] == 'Введённый пароль состоит только из цифр.':
                flag = True
        self.assertTrue(flag)

    def test_signup_page_not_same_password(self):
        response = self.client.post(self.url, {'username': 'testing_user', 'email': 'testingtest@mail.ru',
                                               'password1': 'qwerty12345', 'password2': 'qwerty1234'})
        f = SignUpForm(response.wsgi_request.POST)
        f.is_valid()
        array = f.errors['password2']
        flag = False
        for i in range(len(array)):
            if array[i] == 'Два поля с паролями не совпадают.':
                flag = True
        self.assertTrue(flag)

    def test_signup_page_not_valid_username(self):
        response = self.client.post(self.url, {'username': 'testing user', 'email': 'testingtest@mail.ru',
                                               'password1': 'qwerty12345', 'password2': 'qwerty12345'})
        f = SignUpForm(response.wsgi_request.POST)
        f.is_valid()
        array = f.errors['username']
        flag = False
        for i in range(len(array)):
            if array[i] == 'Введите правильное имя пользователя. Оно может содержать только буквы, цифры и знаки @/./+/-/_.':
                flag = True
        self.assertTrue(flag)


    def test_signup_page_empty_field(self):
        response = [0, 0, 0, 0]
        response[0] = self.client.post(self.url, {'username': '', 'email': 'testilkjhgferest@mail.ru',
                                               'password1': 'qbwertq12345', 'password2': 'qbwertq12345'})
        response[1] = self.client.post(self.url, {'username': 'awa', 'email': '',
                                                'password1': 'qbwertq12345', 'password2': 'qbwertq12345'})
        response[2] = self.client.post(self.url, {'username': 'wwd', 'email': 'testilkjhgferest@mail.ru',
                                                'password1': '', 'password2': 'qbwertq12345'})
        response[3] = self.client.post(self.url, {'username': 'evev', 'email': 'testilkjhgferest@mail.ru',
                                                'password1': 'qbwertq12345', 'password2': ''})
        f = [0, 0, 0, 0]
        for i in range(4):
            f[i] = SignUpForm(response[i].wsgi_request.POST)
            f[i].is_valid()
        for i in range(4):
            array = [f[0].errors['username'], f[1].errors['email'], f[2].errors['password1'], f[3].errors['password2']]
            flag = False
            for j in range(len(array[i])):
                if array[i][j] == 'Обязательное поле.':
                    flag = True
            self.assertTrue(flag)


class VkAuthPage(TestCase):

    def setUp(self):
        self.client = Client()
        self.url = reverse("vk_auth")

    def test_vkauth_page_downloaded(self):
        response = self.client.get(self.url)
        self.assertEqual(response.status_code, 200)

    def test_vkauth_page_title(self):
        response = self.client.get(self.url)
        self.assertEqual(response.context['title'], 'Billy the Messenger')

    def test_vkauth_page_template(self):
        response = self.client.get(self.url)
        self.assertTemplateUsed(response, 'billy_the_messenger/vk_auth.html')

    def test_vkauth_page_empty_fields(self):
        response1 = self.client.post(self.url, {'login': 'testing_user', 'password': ''})
        response2 = self.client.post(self.url, {'login': '', 'password': 'pass'})
        f1 = VkAuthForm(response1.wsgi_request.POST)
        f2 = VkAuthForm(response2.wsgi_request.POST)
        f1.is_valid()
        f2.is_valid()
        array1 = f1.errors['password']
        array2 = f2.errors['login']
        flag1 = False
        for i in range(len(array1)):
            if array1[i] == 'Обязательное поле.':
                flag1 = True
        flag2 = False
        for i in range(len(array2)):
            if array2[i] == 'Обязательное поле.':
                flag2 = True
        self.assertTrue(flag1)
        self.assertTrue(flag2)

    def test_vkauth_page_wrong_data(self):
        response = self.client.post(self.url, {'username': 'testuser', 'password': 'wrongpassword'})
        f = AuthenticationForm(data=response.wsgi_request.POST)
        f.is_valid()
        array = f.errors['__all__']
        flag = False
        for i in range(len(array)):
            if array[i] == 'Пожалуйста, введите правильные имя пользователя и пароль. ' \
                           'Оба поля могут быть чувствительны к регистру.':
                flag = True
        self.assertTrue(flag)


class UserProfilePage(TestCase):

    def setUp(self):
        self.client = Client()
        self.url = reverse('userprofile')

    def test_page_downloaded(self):
        response = self.client.get(self.url)
        self.assertEqual(response.status_code, 302)


class AboutPage(TestCase):

    def setUp(self):
        self.client = Client()
        self.url = reverse('about')

    def test_page_downloaded(self):
        response = self.client.get(self.url)
        self.assertEqual(response.status_code, 200)

    def test_page_template(self):
        response = self.client.get(self.url)
        self.assertTemplateUsed(response, 'billy_the_messenger/about.html')


class TmRegCodePage(TestCase):

    def setUp(self):
        self.client = Client()
        self.url = reverse('tm_reg_code')

    def test_page_downloaded(self):
        response = self.client.get(self.url)
        self.assertEqual(response.status_code, 200)

    def test_page_template(self):
        response = self.client.get(self.url)
        self.assertTemplateUsed(response, 'billy_the_messenger/tm_reg_code.html')


class TmRegPhonePage(TestCase):

    def setUp(self):
        self.client = Client()
        self.url = reverse('tm_reg_phone')

    def test_page_downloaded(self):
        response = self.client.get(self.url)
        self.assertEqual(response.status_code, 200)

    def test_page_template(self):
        response = self.client.get(self.url)
        self.assertTemplateUsed(response, 'billy_the_messenger/tm_reg_phone.html')


class RecoverPassPage(TestCase):

    def setUp(self):
        self.client = Client()
        self.url = reverse('recover_password')

    def test_page_downloaded(self):
        response = self.client.get(self.url)
        self.assertEqual(response.status_code, 200)

    def test_page_template(self):
        response = self.client.get(self.url)
        self.assertTemplateUsed(response, 'billy_the_messenger/recover_password.html')


class TmAuthPage(TestCase):

    def setUp(self):
        self.client = Client()
        self.url = reverse('tm_auth')

    def test_page_downloaded(self):
        response = self.client.get(self.url)
        self.assertEqual(response.status_code, 200)


class TestSendingMsgsTM(TestCase):
    def setUp(self):
        client = tmClient("/tm_sessions/79609856120_session")
        client.start()

    def test_wrong_target(self):
        self.assertRaisers(errors.BadRequest, send_message, self.client.session, "123456789test", "testtesttest")

    def test_client_stopped(self):
        self.client.stop()
        self.assertRaisers(errors.Unauthorized, send_message, self.client.session, "shaponi", "testtesttest")
        self.client.start()

    def test_max_length(self):
        self.assertRaisers(errors.BadRequest, send_message, self.client.session, "shaponi", "a" * 5000)


