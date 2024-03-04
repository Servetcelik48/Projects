"""
Date: 25.06.2021
Created by Servet Çelik

"""
import pygame
import os
import random
pygame.font.init()

# screen features
WIDTH = 750
HEIGHT = 600
WIN = pygame.display.set_mode((WIDTH, HEIGHT))
COOLDOWN = 30 # determines the shooting period

# Load images
# Background
BG = pygame.image.load(os.path.join("assets", "background_space.png"))

# Mission ship
MISSION_SHIP = pygame.image.load(os.path.join("assets", "mission_ship.png"))

# Enemy ships of different colors
RED_ENEMY_SHIP = pygame.image.load(os.path.join("assets", "enemy_ship_red.png"))
GREEN_ENEMY_SHIP = pygame.image.load(os.path.join("assets", "enemy_ship_green.png"))
BLUE_ENEMY_SHIP = pygame.image.load(os.path.join("assets", "enemy_ship_blue.png"))

# Player Laser
PLAYER_LASER = pygame.image.load(os.path.join("assets", "blue_rocket.png"))

# Enemy Lasers of different types
LASER01 = pygame.image.load(os.path.join("assets", "laser01.png"))
LASER02 = pygame.image.load(os.path.join("assets", "laser02.png"))
LASER03 = pygame.image.load(os.path.join("assets", "laser03.png"))

# check collision between two objects
def collide(object1, object2):
	offset_x = object2.x - object1.x
	offset_y = object2.y - object1.y
	return object1.mask.overlap(object2.mask, (offset_x, offset_y)) != None

# Class for laser projectiles
class Laser():
	def __init__(self,x,y,img):
		self.x = x
		self.y = y
		self.img = img
		self.mask = pygame.mask.from_surface(self.img)

	def move(self, velocity):
		self.y += velocity

	def draw(self, window):
		window.blit(self.img, (self.x,self.y))

	# Check collision with another object
	def collision(self, object):
		return collide(object, self)

	# Check if laser is off the screen
	def off_screen(self, height):
		return not(self.y <= height and self.y >= 0)

# Class for general ship properties and methods
class Ship():
	def __init__(self,x,y,health=100):
		self.x = x
		self.y = y
		self.health = health
		self.ship_img = None
		self.laser_img = None
		self.lasers = []
		self.cool_down_counter = 0

	# cooldown while shooting
	def cool_down(self):
		if self.cool_down_counter >= COOLDOWN:
			self.cool_down_counter = 0
		else:
			self.cool_down_counter += 1

	# firing lasers
	def shoot(self):
		if self.cool_down_counter == 0:
			laser = Laser(self.x, self.y, self.laser_img)
			self.lasers.append(laser)

	# Movement of lasers
	def move_lasers(self, velocity, object):
		self.cool_down()
		for laser in self.lasers:
			laser.move(velocity)
			if laser.off_screen(HEIGHT):
				self.lasers.remove(laser)
			elif laser.collision(object):
				object.health -= 10
				self.lasers.remove(laser)

	# Draw ship and lasers
	def draw(self, window):
		window.blit(self.ship_img, (self.x, self.y))
		for laser in self.lasers:
			laser.draw(window)

	# width and height of ship image
	def get_width(self):
		return self.ship_img.get_width()

	def get_height(self):
		return self.ship_img.get_height()

# the player's ship
class PlayerShip(Ship):
	def __init__(self,x,y,health=100):
		super().__init__(x,y,health)
		self.ship_img = MISSION_SHIP
		self.laser_img = PLAYER_LASER
		self.mask = pygame.mask.from_surface(self.ship_img)
		self.max_health = health

	# Override shoot for player ship
	def shoot(self):
		if self.cool_down_counter == 0:
			laser = Laser(self.x+20, self.y, self.laser_img)
			self.lasers.append(laser)

	# Move player ship lasers, removing enemies and lasers if they collide
	def move_lasers(self, velocity, objects):
		self.cool_down()
		for laser in self.lasers:
			laser.move(velocity)
			if laser.off_screen(HEIGHT):
				self.lasers.remove(laser)
			else:
				for object in objects:
					if laser.collision(object):
						objects.remove(object)
						if laser in self.lasers:
							self.lasers.remove(laser)

	# health bar
	def healthbar(self, window):
		pygame.draw.rect(window, (255,0,0), (self.x, self.y + self.ship_img.get_height(),
											 self.ship_img.get_width(), 7))
		pygame.draw.rect(window, (0, 255, 0), (self.x, self.y + self.ship_img.get_height(),
							int(self.ship_img.get_width() * (self.health/self.max_health)), 7))

	# draw health bar
	def draw(self, window):
		window.blit(self.ship_img, (self.x, self.y))
		self.healthbar(window)
		for laser in self.lasers:
			laser.draw(window)

# enemy ships
class EnemyShip(Ship):
	COLOR_MAP = {
		"red": (RED_ENEMY_SHIP, LASER01),
		"green": (GREEN_ENEMY_SHIP, LASER02),
		"blue": (BLUE_ENEMY_SHIP, LASER03)
	}

	def __init__(self,x,y,color,health=100):
		super().__init__(x,y,health)
		self.ship_img, self.laser_img = self.COLOR_MAP[color]
		self.mask = pygame.mask.from_surface(self.ship_img)

	# Movement of enemy ships
	def move(self, velocity):
		self.y += velocity

	# shoot for enemy ships
	def shoot(self):
		if self.cool_down_counter == 0:
			laser = Laser(self.x, self.y, self.laser_img)
			self.lasers.append(laser)

# Main game function
def main():
	run = True
	FPS = 60
	enemies = []  # List to hold enemy ships
	enemy_velocity = 1
	laser_velocity = 5
	enemy_length = 0
	level = 0
	global COOLDOWN
	COOLDOWN = 30

	main_font = pygame.font.SysFont("Algerian", 30)  #display level
	lost_font = pygame.font.SysFont("Algerian", 90)  #display "Game Over"

	player_velocity = 5

	clock = pygame.time.Clock()

	player = PlayerShip(350,450)  # Create player ship
	lost = False
	lost_count = 0

	#draw game elements
	def draws():
		WIN.blit(BG, (0,0))
		player.draw(WIN)
		level_label = main_font.render("LEVEL: {}".format(level), 1, (255, 255, 255))
		WIN.blit(level_label, (600,10))

		for enemy in enemies:
			enemy.draw(WIN)

		if lost:
			lost_label = lost_font.render("GAME OVER!", 1,(200,0,0))
			WIN.blit(lost_label, (int(WIDTH/2 - (lost_label.get_width()/2)), int(HEIGHT/2 - (lost_label.get_height()/2))))

		pygame.display.update()

	# Main game loop
	while run:
		clock.tick(FPS)
		draws()

		# Check if player health is depleted
		if player.health <= 0:
			lost = True
			if lost:
				lost_count += 1
				if lost_count >= FPS * 3:
					run = False
				else:
					continue

		# Check if all enemies are destroyed and increase difficulty and level up
		if len(enemies) == 0:
			enemy_velocity += 1
			laser_velocity += 1
			enemy_length += 5
			level += 1
			COOLDOWN -= 5
			for i in range(enemy_length):
				enemy = EnemyShip(random.randrange(1,700), random.randrange(-1500, -100),
									random.choice(["red","blue","green"]))
				enemies.append(enemy)

		for event in pygame.event.get():
			if event.type == pygame.QUIT:
				run = False

		# Keyboard controls for player movement
		keys = pygame.key.get_pressed()
		if keys[pygame.K_LEFT] and player.x > 0:
			player.x -= player_velocity
		if keys[pygame.K_RIGHT] and player.x < 670:
			player.x += player_velocity
		if keys[pygame.K_UP] and player.y > 0:
			player.y -= player_velocity
		if keys[pygame.K_DOWN] and player.y < 470:
			player.y += player_velocity
		if keys[pygame.K_SPACE]:
			player.shoot()

		# Enemy ship movement and shooting
		for enemy in enemies:
			enemy.move(enemy_velocity)
			enemy.move_lasers(laser_velocity, player)
			if random.randrange(0, 2*60) == 1:
				enemy.shoot()

			# Check for collision with player
			if collide(enemy, player):
				player.health -= 10
				enemies.remove(enemy)
			# Remove enemy if it goes off screen
			if enemy.y > HEIGHT:
				enemies.remove(enemy)

		# Move player ship lasers and remove enemies and lasers if they collide
		player.move_lasers(-laser_velocity, enemies)

# Function to display main menu
def main_menu():
	title_font = pygame.font.SysFont("Algerian", 50)
	run = True

	while run:
		WIN.blit(BG, (0,0))
		main_text = title_font.render("PRESS THE MOUSE TO BEGIN", 1, (255,255,255))
		WIN.blit(main_text, (int(WIDTH/2 - main_text.get_width()/2), int(HEIGHT/2 - main_text.get_height()/2)))

		pygame.display.update()
		for event in pygame.event.get():
			if event.type == pygame.QUIT:
				run = False
			if event.type == pygame.MOUSEBUTTONDOWN:
				main()

	pygame.quit()

main_menu()
