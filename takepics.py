from Tkinter import *
from PIL import Image, ImageTk
from subprocess import call
import os, shutil
import datetime
import dircache
import random
from time import sleep
import ftplib

def takeStandard() :
	startImage = 1
	take = []
	for i in range(1,5):
		shutil.move(datetoday + "/" + newList[i-1],datetoday + "/images/%s" % (newList[i-1]) )
	#	print startImage
		take.append(Image.open(datetoday + "/tempPictures/image%s.JPG" %(startImage)))
		startImage = startImage + 1
		if startImage > 4:
			startImage = 1
	
	featureImage = Image.open(datetoday + "/tempPictures/feature-image.jpg")

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
	
	blank_image.save(datetoday + "/tempPictures/newImage.png")
	
	return blank_image



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

print "Waiting for a quartet of photos!"
count = 0
imageCount = 0
date = 0
newList = []
today = datetime.date.today()
datetoday = '{dt.month}-{dt.day}-{dt.year}'.format(dt = datetime.datetime.now())
tempfilepath = datetoday + "/tempPictures"
processedfilepath = datetoday + "/images"
# if the folder exists, make the folder
if os.path.exists(datetoday):
	
	if not os.path.exists(tempfilepath):
		os.makedirs(tempfilepath)
		print "Make temp folder"
	if not os.path.exists(processedfilepath):
		os.makedirs(processedfilepath)
		print "Make processed folder"
	list = dircache.listdir('./'+datetoday)
	while count < len(list):
	   
	   # find the oldest 4 JPG images and move them into the folder tempPictures 
	   # as well as the images folder
	   
	   if list[count].find("JPG") != -1 and imageCount < 4:
		  filepath = datetoday + "/tempPictures/image%s.JPG" % (imageCount)
		  newList.append(list[count])
		  imageCount = imageCount + 1
		  shutil.copyfile(datetoday + "/" + list[count],datetoday + "/tempPictures/image%s.JPG" % ( imageCount))  
		  
	   count = count + 1

	#print list
	
	if imageCount == 4:
		print "Got photos! Compiling into finished image..."
		printImage = takeStandard()

		# print out the picture - this works
		# uses Windows 7 image viewer to print default size of the paper

		# call('rundll32.exe C:\\WINDOWS\\system32\\shimgvw.dll,ImageView_PrintTo C:\\PhotoBooth\\tempPictures\\newImage.png "MITSUBISHI CP70D Series(USB)"')


		# save the image also in the archive folder
		now = datetime.datetime.now()
		archiveName = "photo_%i-%02i-%02i-%02i-%02i-%02i.png" % (now.year,now.month, now.day, now.hour, now.minute,now.second)
		
		print "Printing %s " % archiveName
		printImage.save(datetoday + "/images/" + archiveName)
	
else:
	sleep(10)
	
