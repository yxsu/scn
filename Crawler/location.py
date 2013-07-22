#!/usr/bin/python
from oauth_helper import *
from datetime import datetime
import sys
import pycurl
import random
import json
import mysql.connector
import logging

#try:
#    from urlparse import parse_qs, parse_qsl
#except ImportError:
#    from cgi import parse_qs, parse_qsl

class FetchLocation:
    twitter_url = 'https://stream.twitter.com/1.1/statuses/filter.json'
    consumer_key = 'YXZm2FQ8mqkxDMKUOpKPpg'
    consumer_secret = 'QYr5cv2HWBRyEFlv55Lp40niY1dOxBOqqmSsOaOCY'
    access_token_key = '142674868-I1yPvLuTAupXc17AB8bDA8GbbnBwdXNgCwytSZmp'
    access_token_secret = '4LRwgGwRtLZUpDvA60kTlwge6kYmoZpLfKKPtiFo'
    log_file = 'log.txt'
    #information about database
    connection_info = {
        'user': 'air',
        'password': 's131413324',
        'host': '10.10.18.233',
        'database': 'check_in',}
    syntax_insert = ("INSERT INTO check_in_data (id_check_in, created_time, check_in_text, source, geo_type, longtitude, latitude, id_user, id_place, id_in_reply_to_status, favorite_count, retweet_count, id_in_reply_to_user) VALUES (%(id_check_in)s, %(created_time)s, %(check_in_text)s, %(source)s, %(geo_type)s, %(longtitude)s, %(latitude)s, %(id_user)s, %(id_place)s, %(id_in_reply_to_status)s, %(favorite_count)s, %(retweet_count)s, %(id_in_reply_to_user)s)")

    def __init__(self):
        #set parameter
        self.parameter = {
            'oauth_version' : "1.0",
            'oauth_nonce' : generate_nonce(),
            'oauth_timestamp' : int(time.time()),
            'oauth_token' : self.access_token_key,
            'oauth_consumer_key' : self.consumer_key,
            'stall_warnings' : 'true'}
        logging.basicConfig(filename = self.log_file, level=logging.DEBUG, 
                            format='[%(asctime)s %(levelname)s] %(message)s')
        #create client
        self.consumer = Consumer(self.consumer_key, self.consumer_secret)
        self.access_token = Token(self.access_token_key, self.access_token_secret)

    def SetSearchRegion(self, coordinate_low, coordinate_high):
        """set the location box for searching. coordinate=[longitude, latitude]"""
        self.parameter['locations'] = str(coordinate_low[0]) + ', ' + str(coordinate_low[1]) +', ' + str(coordinate_high[0]) +', ' + str(coordinate_high[1])
        values = {'locations' : self.parameter['locations'], 'stall_warnings' : 'true'}
        self.post_data = urllib.urlencode(values)
        
    def LocationCallBack(self, buffer):
        """callback function for Curl, the buffer is the twitter stream data
        """
        if not buffer.strip():
            logging.warning('bad string buffer: %s', buffer)
            return -1
        self.SaveToDatabase(buffer)
        
    def SaveToDatabase(self, data_stream):
        """save data with json format into a remote MySql database
        """
            #set database connection
        cursor = self.connector.cursor()
        encoded_json = json.loads(data_stream)
        instance_check_in = {
            'id_check_in': encoded_json['id'],
            'created_time': datetime.strptime(encoded_json['created_at'], '%a %b %d %H:%M:%S +0000 %Y'),
            'check_in_text': encoded_json['text'],
            'source': encoded_json['source'],
            'geo_type': encoded_json['coordinates']['type'],
            'longtitude': encoded_json['coordinates']['coordinates'][0],
            'latitude': encoded_json['coordinates']['coordinates'][1],
            'id_user': encoded_json['user']['id'],
            'id_place': encoded_json['place']['id'],
            'id_in_reply_to_status': encoded_json['in_reply_to_status_id'],
            'favorite_count': encoded_json['favorite_count'],
            'retweet_count': encoded_json['retweet_count'],
            'id_in_reply_to_user': encoded_json['in_reply_to_user_id'],}
            #save
        cursor.execute(self.syntax_insert, instance_check_in)
        self.connector.commit()
        cursor.close()
        
    def Run(self):
        """run the fetching progress forever. 
        The only procedure before this run is to call SetSearchRegion()
        """
        request = Request("POST", self.twitter_url, self.parameter)
        request.sign_request(SignatureMethod_HMAC_SHA1(), 
                             self.consumer, self.access_token)
        headers = ['Authorization: OAuth oauth_consumer_key="' + 
                   request['oauth_consumer_key'] + '", oauth_nonce="' + 
                   request['oauth_nonce'] + '",oauth_signature="'+ 
                   urllib.quote(request['oauth_signature'],'') + 
                   '", oauth_signature_method="HMAC-SHA1", oauth_timestamp="' + 
                   str(request['oauth_timestamp']) + '", oauth_token="' + 
                   request['oauth_token'] +'", oauth_version="1.0"']
        #set Curl
        curl = pycurl.Curl()
        curl.setopt(curl.WRITEFUNCTION, self.LocationCallBack)
        curl.setopt(curl.URL, self.twitter_url)
        curl.setopt(curl.POSTFIELDS, self.post_data)
        curl.setopt(curl.VERBOSE, True)
        curl.setopt(curl.HTTPHEADER, headers)
        #run
        try:
            self.connector = mysql.connector.connect(user='air', password='s131413324',
                                                     host = '10.10.18.233', database='check_in')
            curl.perform()
        except pycurl.error, error:
            curl.close()
            self.connector.close()
            logging.warning(str(error))
        except:
            curl.close()
            self.connector.close()
            logging.warning(str(sys.exc_info()[0]))


if __name__ == '__main__':
    crawler = FetchLocation()
    crawler.SetSearchRegion([-74.5, 40.5], [-73, 41])
    crawler.Run()



















