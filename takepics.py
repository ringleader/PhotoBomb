from Tkinter import *
from PIL import Image, ImageTk
from subprocess import call
import os, shutil
import datetime
import dircache
import random
from time import sleep
import ftplib

# takes 4 pictures and converts them to png files
# then combines all 4 pictures, saves it in the images folder
# and prints out the image to the dye sub printer

""""
# This section creates a tinyURL for the uploaded files
from urllib import urlencode
from pyshorteners import Shortener

url = 'http://docs.python-requests.org/en/master/user/install/#install'
shortener = Shortener('Tinyurl')
print "My short url is {}".format(shortener.short(url))

"""

# This section opens an FTP connection and uploads the photo
file = "image1.JPG"
""""
from fabric.api import env
from fabric.operations import run, put

env.hosts = ['home296064887.1and1-data.host']
# env.user = 'u54648292-photobomb'
# env.password = 'FirePictures47!'
env.user = 'u54648292'
env.password = 'L_square8'
def copy():
    # make sure the directory is there!
    run('mkdir -p farm/photoBomb')

    # our local 'testdirectory' - it may contain files or subdirectories ...
    put(file, '/photoBomb')
copy()

# more different not working code
import pysftp

def upload_file(file_path):

    private_key = "L_square8"  # can use password keyword in Connection instead
    srv = pysftp.Connection(host="whatsgoodthere.com", username="u54648292", private_key=private_key)
    srv.chdir('/farm/photoBomb')  # change directory on remote server
    srv.put(file_path)  # To download a file, replace put with get
    srv.close()  # Close connection
	
upload_file(file)	
"""	

print "Waiting for a quartet of photos!"
list = dircache.listdir('.')
count = 0
imageCount = 0
newList = []
while count < len(list):
   
   # find the oldest 4 JPG images and move them into the folder tempPictures 
   # as well as the images folder
   
   if list[count].find("JPG") != -1 and imageCount < 4:
      #print list[count]
      newList.append(list[count])
      imageCount = imageCount + 1
      shutil.copyfile(list[count],"tempPictures/image%s.JPG" % ( imageCount))  
      
   count = count + 1

#print list
take = []
if imageCount == 4:
	#startImage = random.randint(1,4)
	startImage = 1
	print "Got photos! Compiling into finished image..."
	
	for i in range(1,5):
		shutil.move(newList[i-1],"images/%s" % (newList[i-1]) )
	#	print startImage
		take.append(Image.open("tempPictures/image%s.JPG" %(startImage)))
		startImage = startImage + 1
		if startImage > 4:
			startImage = 1
	
	featureImage = Image.open("tempPictures/featurd-image.svg")

	imageWidthBig = 1830	
	imageHeightBig = int(imageWidthBig * .75)
	imageWidth = 1100
	imageHeight = int(imageWidth * .75 )
	imageSpacing = 35
	BottomRowTop = imageHeightBig +  imageSpacing * 2



	take[0]=take[0].resize((imageWidthBig, imageHeightBig), Image.NEAREST)
	take[1]=take[1].resize((imageWidth,imageHeight), Image.NEAREST)
	take[2]=take[2].resize((imageWidth,imageHeight), Image.NEAREST)
	take[3]=take[3].resize((imageWidth,imageHeight), Image.NEAREST)

	blank_image = Image.new("RGB", (3456, 2304), "white")
	blank_image.paste(featureImage,(2300,imageSpacing))
	blank_image.paste(take[0], (imageSpacing,imageSpacing))
	blank_image.paste(take[1], (imageSpacing,BottomRowTop))
	blank_image.paste(take[2], (imageSpacing * 2 + imageWidth ,BottomRowTop))
	blank_image.paste(take[3], (imageSpacing * 3 + imageWidth * 2,BottomRowTop))

	blank_image.save("tempPictures/newImage.png")

	# print out the picture - this works
	# uses Windows 7 image viewer to print default size of the paper

	# call('rundll32.exe C:\\WINDOWS\\system32\\shimgvw.dll,ImageView_PrintTo C:\\PhotoBooth\\tempPictures\\newImage.png "MITSUBISHI CP70D Series(USB)"')


	# save the image also in the archive folder
	now = datetime.datetime.now()
	archiveName = "photo_%i-%02i-%02i-%02i-%02i-%02i.png" % (now.year,now.month, now.day, now.hour, now.minute,now.second)
	
	print "Printing %s " % archiveName
	blank_image.save("images/" + archiveName)
	
else:
	sleep(10)
