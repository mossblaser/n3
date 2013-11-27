import Image, ImageDraw


class HD44780Renderer(object):
	
	def __init__( self
	            , width = 8, height = 2
	            , pixel_size = 4
	            , pixel_space = 1
	            , char_space = 5
	            , border_space = 12
	            , pixel_on_colour = (5,5,5)
	            , pixel_off_colour = (104,136,7)
	            , background_colour = (124,163,4)
	            ):
		self.width = width
		self.height = height
		self.pixel_size  = pixel_size
		self.pixel_space = pixel_space
		self.char_space  = char_space
		self.border_space = border_space
		self.pixel_on_colour = pixel_on_colour
		self.pixel_off_colour = pixel_off_colour
		self.background_colour = background_colour
	
	def _init_image(self):
		self.image = Image.new("RGB", (
			self.width * ((self.pixel_size + self.pixel_space)*5 - self.pixel_space + self.char_space) - self.char_space
			  + self.border_space*2
			,
			self.height * ((self.pixel_size + self.pixel_space)*8 - self.pixel_space + self.char_space) - self.char_space
			  + self.border_space*2
		))
			
		self.drawing = ImageDraw.Draw(self.image)
		self.drawing.rectangle(((0,0), self.image.size), fill = self.background_colour)
	
	
	def _finalise_image(self):
		del self.drawing
		return self.image
	
	
	def _to_px(self, x, y):
		col = x/6
		row = y/9
		char_x = x%6
		char_y = y%9
		
		out_x  = self.border_space
		out_y  = self.border_space
		
		out_x += col * ((self.pixel_size + self.pixel_space)*5 - self.pixel_space + self.char_space)
		out_y += row * ((self.pixel_size + self.pixel_space)*8 - self.pixel_space + self.char_space)
		
		out_x += char_x*(self.pixel_size + self.pixel_space)
		out_y += char_y*(self.pixel_size + self.pixel_space)
		
		return (out_x, out_y)
	
	
	def _draw_pixel(self, x, y, state):
		# Don't draw intermediate pixels
		if x%6 == 5 or y%9 == 8:
			return
		
		x, y = self._to_px(x,y)
		
		self.drawing.rectangle( ((x,y), (x+self.pixel_size-1, y+self.pixel_size-1))
		                      , fill = self.pixel_on_colour if state else self.pixel_off_colour
		                      )
	
	
	def render(self, im):
		self._init_image()
		
		for x in range(im.size[0]):
			for y in range(im.size[1]):
				self._draw_pixel(x,y, im.getpixel((x,y)))
		
		return self._finalise_image()



if __name__=="__main__":
	import sys
	r = HD44780Renderer()
	r.render(Image.open(sys.argv[1])).save(sys.argv[2])
