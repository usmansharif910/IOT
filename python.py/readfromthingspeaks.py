import urllib
from bs4 import BeautifulSoup

datafromthingspeakschannel=urllib.urlopen("https://api.thingspeak.com/channels/1751503/feeds.json?api_key=GDG5HCZ6DISBUUC1&results=2");


print (datafromthingspeakschannel.raed());

# print( "Usman")