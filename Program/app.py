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
        can_img0 = self.get_argument(name='imgBase64')
        can_img1 = can_img0[can_img0.find(',') + 1:]
        can_img2 = Image.open(StringIO(base64.decodestring(can_img1)))
        test = multi_to_single_channel(numpy.array(can_img2), 3)
        full_image = Image.fromarray(numpy.array(test).reshape(200, 400),mode="L").transpose(Image.FLIP_LEFT_RIGHT)
        full_image = full_image.transpose(Image.ROTATE_270)
        vectors = py_dbscan.get_vectors(full_image,1,1)
        s=""
        for vector in vectors:
            s=str(classifier.predict(vector)[0])+s
        json_obj = {}
        json_obj['ret']= s
        self.write(json_obj)

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
