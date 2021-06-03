# Python 3
import http.client
import urllib.parse
import sys

conn = http.client.HTTPConnection('api.positionstack.com')

params = urllib.parse.urlencode({
    'access_key': 'a12da357b9a5be8da6613efebd21fa5e',
    'query': sys.argv[1:],
    'country': 'BE',
    'limit': 1,
})

conn.request('GET', '/v1/forward?{}'.format(params))

res = conn.getresponse()
data = res.read().decode('utf-8')

with open("location.txt", "w") as f:
    write = ""
    start = data.find("latitude")
    end = data.find(",", start)
    write += "" + data[start + 10:end] + ","
    start = data.find("longitude")
    end = data.find(",", start)
    write += data[start + 11:end]
    f.write(write)
