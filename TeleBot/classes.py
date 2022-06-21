class SubType:
    __type = ''

    @property
    def type(self):
        return self.__type

    @type.setter
    def type(self, st):
        self.__type = st


class Client:
    def __init__(self, name, subtype):
        self.__name = name
        self.__subType = subtype

    @property
    def name(self):
        return self.__name

    @property
    def subType(self):
        return self.__subType

    __like_reviews = {
        'likes': 0,
        'dislikes': 0
    }

    @property
    def like_reviews(self):
        return self.__like_reviews

    @like_reviews.setter
    def like_reviews(self, evaluation):
        if evaluation == 'like':
            self.__like_reviews['likes'] += 1
        elif evaluation == 'dislike':
            self.__like_reviews['dislikes'] += 1

    star_reviews = {}

    def getMeanSR(self):
        mean = 0
        for x in self.star_reviews.values():
            for mark in x:
                mean += int(mark)
        mean /= len(list(self.star_reviews.values())[0])
        return str(mean)

    text_reviews = {}

    fbID = 0
    def printFP(self):
        STR = f'Average review score: {self.getMeanSR()}\n'
        for item in list(self.text_reviews.values())[0]:
            STR += f'Feedback ID: #{self.fbID}\n{item}\n\n'
            self.fbID += 1
        return STR


class Review:
    def __init__(self, like, rating, fb):
        self.__like = like
        self.__rating = rating
        self.__feedback = fb

    @property
    def like(self):
        return self.__like

    @like.setter
    def like(self, like):
        if like == 'like' or like == 'dislike':
            self.__like = like
        else:
            print('Invalid input data')

    @property
    def rating(self):
        return self.__rating

    @rating.setter
    def rating(self, rating):
        if rating in range(1, 6):
            self.__rating = rating
        else:
            print('Invalid input data')

    @property
    def feedback(self):
        return self.__feedback

    @feedback.setter
    def feedback(self, fb):
        if type(fb) == type('String'):
            self.__feedback = fb
        else:
            print('Invalid input data')


class FeedBacker:
    def sendReview(self, review):
        review.send_to_database(save=True)


class DataBase:
    def __init__(self, clients, reviews):
        self.__clients = clients
        self.__reviews = reviews

    def addData(self, rev):
        if len(rev) in range(0, 100):
            self.__reviews.add(rev)
        else:
            print('Invalid input data')

    def addClient(self, client):
        if client.SubType == 'LightP' or client.SubType == 'FullP':
            self.__clients.add(client, client.SubType)
        else:
            print('Invalid input data')

    def getReviews(self, client):
        return self.__reviews.filter(client=client)
