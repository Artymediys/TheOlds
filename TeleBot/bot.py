import telebot
import BotInfo
from classes import Client
from classes import DataBase
from classes import FeedBacker
from classes import Review
from classes import SubType
from telebot import types

# Functional class for our Telegram Bot
TGBot = telebot.TeleBot(BotInfo.config['token'])

# global variables
userID = ''
compName = ''
subType = ''
flag = ''
key = ''
mark = ''


@TGBot.message_handler(commands=['start', 'help', 'info', 'menu'])
def subs(message):
    global userID
    userID = message.from_user.id

    inline_keyboard = types.InlineKeyboardMarkup()
    ans_client = types.InlineKeyboardButton(text='Client', callback_data='client')
    ans_fb = types.InlineKeyboardButton(text='FeedBacker', callback_data='FB')
    inline_keyboard.add(ans_client, ans_fb)

    TGBot.send_message(message.chat.id,
                        'Menu:\n' +
                        'Client - go to the interface of the client (business)\n' +
                        'FeedBacker - go to the interface of the feedbacker\n\n' +
                        'P.S.: To return to the selection menu afterwards, write /menu',
                       reply_markup=inline_keyboard)


@TGBot.callback_query_handler(func=lambda call: True)
def answer(call):
    global flag, subType
    # Client (business)
    if call.data == 'client':
        if userID in BotInfo.clients:
            markup_reply = types.InlineKeyboardMarkup()
            get_stat = types.InlineKeyboardButton(text='Get feedback', callback_data='getFB')
            markup_reply.add(get_stat)

            TGBot.send_message(call.message.chat.id,
                                'To see reviews click on the button below',
                               reply_markup=markup_reply)
        else:
            markup_reply = types.InlineKeyboardMarkup()
            ans_lp = types.InlineKeyboardButton(text='Light-package', callback_data='LP')
            ans_fl = types.InlineKeyboardButton(text='Full-package', callback_data='FP')
            markup_reply.add(ans_lp, ans_fl)

            TGBot.send_message(call.message.chat.id,
                                'We offer two subscription options:\n' +
                                'Light-package – feedbackers will be able to leave feedback ' +
                                'in the form of likes and dislikes, and you will receive collected statistics.\n' +
                                'Full-package - feedbackers will be able to leave feedback ' +
                                'in the form of a score from 1 to 5 + text comment, and you can view ' +
                                'all the information collected in the form of a table.')
            TGBot.send_message(call.message.chat.id, 'Choose one of the subscription options',
                               reply_markup=markup_reply)
    elif call.data == 'getFB':
        if BotInfo.clients[userID].subType == 'LP':
            TGBot.send_message(call.message.chat.id, 'Likes: ' + str(BotInfo.clients[userID].like_reviews['likes']) +
                                '\nDislikes: ' + str(BotInfo.clients[userID].like_reviews['dislikes']))
        elif BotInfo.clients[userID].subType == 'FP':
            TGBot.send_message(call.message.chat.id, str(BotInfo.clients[userID].printFP()))

    # Light-pack subscription
    elif call.data == 'LP':
        markup_reply = types.InlineKeyboardMarkup()
        ans_trial = types.InlineKeyboardButton(text='Confirm', callback_data='confirmLP')
        markup_reply.add(ans_trial)

        TGBot.send_message(call.message.chat.id, 'Since you are a new user, the first 30 days of bot use for free!',
                           reply_markup=markup_reply)
    elif call.data == 'confirmLP':
        flag, subType = 'business', 'LP'
        TGBot.send_message(call.message.chat.id, 'Write the name of your company, ' +
                            'by which users will search for it.\n' +
                            'The company name must be between 1 and 99 characters in length!')

    # Full-pack subscription
    elif call.data == "FP":
        markup_reply = types.InlineKeyboardMarkup()
        ans_trial = types.InlineKeyboardButton(text='Confirm', callback_data='confirmFP')
        markup_reply.add(ans_trial)

        TGBot.send_message(call.message.chat.id, 'Since you are a new user, the first 30 days of bot use for free!',
                           reply_markup=markup_reply)
    elif call.data == 'confirmFP':
        flag, subType = 'business', 'FP'
        TGBot.send_message(call.message.chat.id, 'Write the name of your company, ' +
                            'by which users will search for it.\n' +
                            'The company name must be between 1 and 99 characters in length!')

    # FeedBacker
    if call.data == 'FB':
        flag = 'feedbacker'
        TGBot.send_message(call.message.chat.id, 'Write the name of the company you want to leave a review for.\n')

    elif call.data == 'like':
        BotInfo.clients[key].like_reviews = 'like'
        TGBot.send_message(call.message.chat.id, 'Congratulations, your review has been sent!')
    elif call.data == 'dislike':
        BotInfo.clients[key].like_reviews = 'dislike'
        TGBot.send_message(call.message.chat.id, 'Congratulations, your review has been sent!')


@TGBot.message_handler(content_types=['text'])
def getText(message):
    matched = 'no'
    global flag, key, mark
    if flag == 'business':
        if len(message.text) in range(1, 100):
            global compName
            compName = message.text
            user = Client(compName, subType)
            BotInfo.clients[userID] = user
            BotInfo.clients[userID].star_reviews[userID] = []
            BotInfo.clients[userID].text_reviews[userID] = []
            flag = 0
            TGBot.send_message(message.chat.id, 'Congratulations! Now you can view the feedback ' +
                                'left by users of your company directly in the bot!')
        else:
            TGBot.send_message(message.chat.id, 'The name of the company does not fit' +
                                'the conditions, enter another name!')
    elif flag == 'feedbacker':
        for key, value in BotInfo.clients.items():
            if value.name == message.text and value.subType == 'LP':
                markup_reply = types.InlineKeyboardMarkup()
                ans_like = types.InlineKeyboardButton(text='Like', callback_data='like')
                ans_dislike = types.InlineKeyboardButton(text='Dislike', callback_data='dislike')
                markup_reply.add(ans_like, ans_dislike)

                TGBot.send_message(message.chat.id, 'Give the company a LIKE or DISLIKE', reply_markup=markup_reply)
                flag = 0
                break
            elif value.name == message.text and value.subType == 'FP':
                keyboard_reply = types.ReplyKeyboardMarkup(resize_keyboard=True)
                ans1 = types.KeyboardButton('1')
                ans2 = types.KeyboardButton('2')
                ans3 = types.KeyboardButton('3')
                ans4 = types.KeyboardButton('4')
                ans5 = types.KeyboardButton('5')
                keyboard_reply.add(ans1, ans2, ans3, ans4, ans5)
                TGBot.send_message(message.chat.id, 'Rate the company from 1 to 5', reply_markup=keyboard_reply)
                flag = 'fbMark'
                break
            else:
                TGBot.send_message(message.chat.id, 'This company is not in our database!')
    elif flag == 'fbMark':
        mark = message.text
        TGBot.send_message(message.chat.id, 'Write a text review')
        flag = 'fbText'
    elif flag == 'fbText':
        BotInfo.clients[key].star_reviews[key].append(mark)
        BotInfo.clients[key].text_reviews[key].append(message.text)
        TGBot.send_message(message.chat.id, 'Congratulations, your review has been sent!')
        flag = 0
    elif flag == 0:
        TGBot.send_message(message.chat.id, 'Please follow the instructions!')


TGBot.polling(none_stop=True, interval=0)
