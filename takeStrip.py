from Tkinter import *
from PIL import Image, ImageTk, ImageFont, ImageDraw
from subprocess import call
import os, shutil
import datetime
import dircache
import random
from time import sleep
import ftplib
import facebook
from image2gif import writeGif

bottomText = "Flaming Photobooth"
printPhotos = False
postFBPhotos = False
makeGif = True
fbMessage = 'The Flaming Photobooth just got access to facebook!'
fbpageID = "pageIdHere"
fbaccesstoken = "accessTokenHere"

def takeStrip() : 
	startImage = 1
	take = []
	for i in range(1,5):
		shutil.move(datetoday + "/" + newList[i-1],datetoday + "/images/%s" % (newList[i-1]) )
		take.append(Image.open(datetoday + "/tempPictures/image%s.JPG" %(startImage)))
		startImage = startImage + 1
		if startImage > 4:
			startImage = 1
	
	# featureImage = Image.open(datetoday + "/tempPictures/feature-image.jpg")
	randomBG = int(random.randint(1,3)-1)
	backgrounds = ["blueStripeBg.jpg","multiBg.jpg","stripedBg.jpg"]
	
	font = ImageFont.truetype("Firefly.otf", 45)
	imageWidth = 545
	imageHeight = int(imageWidth * .75)
	imageSpacing = 20
	imageSpacingHorizontal = 35
	secondRow = imageHeight +  (imageSpacing * 2)
	thirdRow = (imageHeight * 2) +  (imageSpacing * 3)
	fourthRow = (imageHeight * 3) +  (imageSpacing * 4)
	logo = (imageHeight * 4) +  (imageSpacing * 3)
		
	take[0]=take[0].resize((imageWidth,imageHeight), Image.NEAREST)
	take[1]=take[1].resize((imageWidth,imageHeight), Image.NEAREST)
	take[2]=take[2].resize((imageWidth,imageHeight), Image.NEAREST)
	take[3]=take[3].resize((imageWidth,imageHeight), Image.NEAREST)
	
	#blank_image = Image.open(backgrounds[randomBG]) # Orange and blue stripes
	blank_image = Image.open("damask.jpg")
	
	blank_image.paste(take[0], (imageSpacingHorizontal,imageSpacing))
	blank_image.paste(take[0], (imageSpacingHorizontal * 2 + imageWidth,imageSpacing))
	blank_image.paste(take[1], (imageSpacingHorizontal,secondRow))
	blank_image.paste(take[1], (imageSpacingHorizontal * 2 + imageWidth,secondRow))
	blank_image.paste(take[2], (imageSpacingHorizontal,thirdRow))
	blank_image.paste(take[2], (imageSpacingHorizontal * 2 + imageWidth,thirdRow))
	blank_image.paste(take[3], (imageSpacingHorizontal,fourthRow))
	blank_image.paste(take[3], (imageSpacingHorizontal * 2 + imageWidth,fourthRow))
	blank_image.paste(take[3], (imageSpacingHorizontal,fourthRow))
	blank_image.paste(take[3], (imageSpacingHorizontal * 2 + imageWidth,fourthRow))
	logoText = ImageDraw.Draw(blank_image)
	#logoText.text((120, 1700), bottomText, font=font, fill="white")
	#logoText.text((int(imageWidth + (imageSpacingHorizontal * 2) + 85), 1700), bottomText, font=font, fill="white")
	logoText.text((160, 1700), datetoday, font=font, fill="white")
	logoText.text((int(imageWidth + (imageSpacingHorizontal * 2) + 135), 1700), datetoday, font=font, fill="white")
	logoText = ImageDraw.Draw(blank_image)
	blank_image.save(datetoday + "/tempPictures/newImage.png")
	
	if makeGif == True:
		archiveGifName = datetoday + "/images/photo_%i-%02i-%02i-%02i-%02i-%02i.gif" % (now.year,now.month, now.day, now.hour, now.minute,now.second)
		gifStrip = writeGif(archiveGifName, take, duration=.75, dither=0)
	
	return blank_image
	
def get_api(cfg):
	graph = facebook.GraphAPI(cfg['access_token'])
	resp = graph.get_object('me/accounts')
	page_access_token = None
	for page in resp['data']:
		if page['id'] == cfg['page_id']:
			page_access_token = page['access_token']
		graph = facebook.GraphAPI(page_access_token)
	return graph


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
now = datetime.datetime.now()
datetoday = '{dt.month}-{dt.day}-{dt.year}'.format(dt = datetime.datetime.now())
tempfilepath = datetoday + "/tempPictures"
processedfilepath = datetoday + "/images"
# facebook




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
		printImage = takeStrip()

		# print out the picture - this works
		# uses Windows 7 image viewer to print default size of the paper
		command = ('rundll32.exe C:\WINDOWS\system32\shimgvw.dll,ImageView_PrintTo C:\Users\James\Pictures\Eye-Fi\%s' % (datetoday) )+ r'\tempPictures\newImage.png "Canon SELPHY CP1200 WS"'
		print command
		if printPhotos == True:
			call(command)


		# save the image also in the archive folder
		now = datetime.datetime.now()
		archiveName = "photo_%i-%02i-%02i-%02i-%02i-%02i.png" % (now.year,now.month, now.day, now.hour, now.minute,now.second)
		
		print "Printing %s " % archiveName
		printImage.save(datetoday + "/images/" + archiveName)
		
		"""
		# get longer lived token
		if postFBPhotos == True:
			api.put_photo(image=open(photoPath,'rb').read(), message=fbMessage)
			cfg = {
				"page_id"      : fbpageID,
				"access_token" : fbaccesstoken
				}
			photoPath = (datetoday) + r'\tempPictures\newImage.png'
			api = get_api(cfg)
			msg = "The Flaming Photobooth just got access to facebook!"
		"""
	
else:
	sleep(10)
	
