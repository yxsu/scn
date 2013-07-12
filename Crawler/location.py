import random
import time

twitter_url = 'https://stream.twitter.com/1.1/statuses/filter.json'
consumer_key = 'YXZm2FQ8mqkxDMKUOpKPpg'
consumer_secret = 'QYr5cv2HWBRyEFlv55Lp40niY1dOxBOqqmSsOaOCY'
access_token_key = '142674868-I1yPvLuTAupXc17AB8bDA8GbbnBwdXNgCwytSZmp'
access_token_secret = '4LRwgGwRtLZUpDvA60kTlwge6kYmoZpLfKKPtiFo'

def generate_nonce(length = 32):
    """generate a string of pseudorandom number"""
    return ''.join([str(random.randint(0, 9)) for i in range(length)])

parameter = {
    'oauth_version' : "1.0",
    'oauth_nonce' : generate_nonce(),
    'oauth_timestamp' : int(time.time())}

