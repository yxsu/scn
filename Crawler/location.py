#!/usr/bin/python
from oauth_helper import *
from datetime import datetime
import sys
import pycurl
import random


try:
    from urlparse import parse_qs, parse_qsl
except ImportError:
    from cgi import parse_qs, parse_qsl


twitter_url = 'https://stream.twitter.com/1.1/statuses/filter.json'
consumer_key = 'YXZm2FQ8mqkxDMKUOpKPpg'
consumer_secret = 'QYr5cv2HWBRyEFlv55Lp40niY1dOxBOqqmSsOaOCY'
access_token_key = '142674868-I1yPvLuTAupXc17AB8bDA8GbbnBwdXNgCwytSZmp'
access_token_secret = '4LRwgGwRtLZUpDvA60kTlwge6kYmoZpLfKKPtiFo'

#set properties
parameter = {
    'oauth_version' : "1.0",
    'oauth_nonce' : generate_nonce(),
    'oauth_timestamp' : int(time.time()),
    'oauth_token' : access_token_key,
    'oauth_consumer_key' : consumer_key,
    'stall_warnings' : 'true'}

consumer = Consumer(consumer_key, consumer_secret)
access_token = Token(access_token_key, access_token_secret)

parameter['locations'] = '-74.98, 39.77, -72.98, 41.77'
values = {'locations' : '-74.98, 39.77, -72.98, 41.77', 'stall_warnings' : 'true'}
post_data = urllib.urlencode(values)

request = Request("POST", twitter_url, parameter)
request.sign_request(SignatureMethod_HMAC_SHA1(), consumer, access_token)

headers = ['Authorization: OAuth oauth_consumer_key="' + request['oauth_consumer_key'] + '", oauth_nonce="' + request['oauth_nonce'] + '",oauth_signature="'+ urllib.quote(request['oauth_signature'],'') + '", oauth_signature_method="HMAC-SHA1", oauth_timestamp="' + str(request['oauth_timestamp']) + '", oauth_token="' + request['oauth_token'] +'", oauth_version="1.0"']

#set data stream
time_max = 600
time_begin = 60
time_stall = 60
class Stream:
    def __init__(self):
        self.read = ''
        self.count = 0
    def body_callback(self,buf):
        time_stall = time_begin
        now = datetime.now()
        hour = now.hour
        minute = now.minute
        if not buf.strip():
            print 'bad string buf:' + buf
            self.count = self.count + 1
            print 'count is :' + str(self.count)
            time.sleep(5)
            if self.count > 10:
                return -1
        else:
            self.count = 0
        if (hour == 5 and minute == 17):
            return -1
        f = open('/Users/suyuxin/Downloads/test/NY_3','a')
        f.write(buf)
        f.flush()
        f.close()
err_file = open('/Users/suyuxin/Downloads/test/error','w')
data_stream = Stream()
#set Curl
curl = pycurl.Curl()
curl.setopt(curl.WRITEFUNCTION, data_stream.body_callback)
curl.setopt(curl.URL, twitter_url)
curl.setopt(curl.POSTFIELDS, post_data)
curl.setopt(curl.VERBOSE, True)
curl.setopt(curl.HTTPHEADER, headers)

try:
    curl.perform()
    time_stall = time_begin
except pycurl.error, error:
    errno, errstr = error
    curl.close()
    print 'An error occurred: ', errstr
    err_file.write(errstr + '\n')
    err_file.flush()
    print "going to sleep %d" % (time_stall)
    time.sleep(time_stall)
    time_stall = time_stall + 60
    if time_stall > time_max:
        time_stall = time_max
except:
    curl.close()
    print "Unexcepted error:" , sys.exc_info()[0]
    err_file.write(str(sys.exc_info()[0]) + '\n')
    err_file.flush()
    print "going to sleep %d" % (time_stall)
    time.sleep(time_stall)
    time_stall = time_stall + 60
    if time_stall > time_max:
        time_stall = time_max
error.close()
