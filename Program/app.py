import os.path
from PIL import Image
from StringIO import StringIO
import tornado.httpserver
import tornado.ioloop
import tornado.web
import base64
import py_dbscan
import classifier

import numpy
from astropy.units import pix

classifier = classifier.Classifier()

def multi_to_single_channel(image_array, channel):
    new_image_array = []  # change this probably
    for row in image_array:
        for pixel in row:
            new_image_array.append(pixel[channel])
    return new_image_array


class MainHandler(tornado.web.RequestHandler):
    def get(self):
        self.render("index.html"
                    )

    def post(self):
        a = self.get_argument(name='imgBase64')
        b = a[a.find(',') + 1:]
        c = Image.open(StringIO(base64.decodestring(b)))
        test = multi_to_single_channel(numpy.array(c), 3)
        foo = Image.fromarray(numpy.array(test).reshape(200, 400),mode="L").transpose(Image.FLIP_LEFT_RIGHT)
        foo = foo.transpose(Image.ROTATE_270)
        vectors = py_dbscan.get_vectors(foo,1,1)
        s=""
        for vector in vectors:
            s+=str(classifier.predict(vector))
        print s


settings = dict(
                template_path=os.path.join(os.path.dirname(__file__), "templates"),
                static_path=os.path.join(os.path.dirname(__file__), "static"),
                debug=True
)

application = tornado.web.Application([
        (r"/home", MainHandler)
], **settings)

if __name__ == "__main__":
    print("Server Running...")
    print("Press ctrl + c to close")
    application.listen(8889)
    tornado.ioloop.IOLoop.instance().start()
