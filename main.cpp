
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sfml/Graphics.hpp>

////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
enum KeyType
{
	UP,DOWN,RIGHT,LEFT,SPACE
};
typedef std::map<KeyType, bool> mapType;

void SetMap(mapType & type, const sf::RenderWindow & win)
{
	type[UP]	= win.GetInput().IsKeyDown(sf::Key::Up);
	type[DOWN]  = win.GetInput().IsKeyDown(sf::Key::Down);
	type[RIGHT] = win.GetInput().IsKeyDown(sf::Key::Right);
	type[LEFT]  = win.GetInput().IsKeyDown(sf::Key::Left);
	type[SPACE] = win.GetInput().IsKeyDown(sf::Key::Space);
	/*type[PUP]	= win.GetInput().IsKeyDown(sf::Key::W);
	type[PDOWN]  = win.GetInput().IsKeyDown(sf::Key::A);
	type[PRIGHT] = win.GetInput().IsKeyDown(sf::Key::S);
	type[PLEFT]  = win.GetInput().IsKeyDown(sf::Key::D);
	type[PSPACE] = win.GetInput().IsKeyDown(sf::Key::R);*/
}

const int ScreenWidth  = 600;
const int ScreenHeight = 500;
const int Speed		   = 300;

class Player
{
public:
	Player( const std::string & s) : frame(0)
	{
		static sf::Image image = sf::Image();
		image.LoadFromFile(s);
		
		sprite.SetImage(image);
		sprite.SetSubRect(	sf::IntRect(0,  61, 32, 127) );
		sprite.SetPosition(400,400);
		
		int x = 0;
		int y = 32;
		for(int i = 0; i < 6; ++i)
		{
			//Sub rects of the sprite sheets
			leftSprites.push_back(sf::IntRect(x, 61, y, 127));
			rightSprites.push_back(sf::IntRect(x, 192, y, 255));
			upSprites.push_back(sf::IntRect(x, 126, y, 192));
			downSprites.push_back(sf::IntRect(x,0,y,62));
			
			x += 32;
			y += 32;
		}
	}
	void Move(const sf::RenderWindow & App)
	{
		WallCollision(App.GetFrameTime());
		SetMap(easierKey, App);
		
		float delta = App.GetFrameTime() * spriteSpeed;
		
		if(easierKey[LEFT])
		{
			sprite.SetSubRect(leftSprites[frame]);
			sprite.Move(-delta,0);
			++frame;
		}
		if(easierKey[RIGHT])
		{
			sprite.SetSubRect(rightSprites[frame]);
			sprite.Move(delta,0);
			++frame;
		}
		if(easierKey[UP])
		{
			sprite.SetSubRect(upSprites[frame]);
			sprite.Move(0,-delta);
			++frame;
		}
		if(easierKey[DOWN])
		{
			sprite.SetSubRect(downSprites[frame]);
			sprite.Move(0, delta);
			++frame;
		}
		//Return to first frame
		
		if(frame >= leftSprites.size() - 1)
			frame = 0;
	}
	void Show( sf::RenderWindow & App)
	{
		App.Draw(sprite); 
	}
private:
	void WallCollision(float delta)
	{
		const float x = sprite.GetPosition().x;
		const float y = sprite.GetPosition().y;
		
		static const float WidthMinus  = ScreenWidth  - sprite.GetSize().x;
		static const float HeightMinus = ScreenHeight - sprite.GetSize().y;
		
		if(x <= 0) 	sprite.Move(spriteSpeed * delta, 0);
		if(y <= 0)	sprite.Move(0, spriteSpeed * delta);
		
		if(y >= HeightMinus) sprite.Move(0, -spriteSpeed * delta);
		if(x >= WidthMinus)  sprite.Move(-spriteSpeed * delta, 0);
	}
private:
	sf::Sprite sprite;
	enum {spriteSpeed = 200};
	
	//Holds the sub rects of the sprites
	std::vector<sf::IntRect> leftSprites;
	std::vector<sf::IntRect> rightSprites;
	std::vector<sf::IntRect> upSprites;
	std::vector<sf::IntRect> downSprites;
	
	mapType easierKey;
	
	unsigned int frame;
};
class Background
{
public:
	Background(const std::string & s) 
	{
		static sf::Image image = sf::Image();
		image.LoadFromFile(s);
		sprite.SetImage(image);
		sprite.SetPosition(0,0);
		
		back.SetImage(image);
		back.SetPosition(-sprite.GetSize().x + 10,0);
	}
	void Move(const sf::RenderWindow & App)
	{
		const float delta = App.GetFrameTime();
		sprite.Move(backSpeed * delta, 0);
		back.Move(backSpeed * delta, 00);
		
		if(sprite.GetPosition().x >= ScreenWidth)
			sprite.SetPosition(-sprite.GetSize().x + 10, 0);
		if(back.GetPosition().x >= ScreenWidth)
			back.SetPosition(-sprite.GetSize().x + 10, 0);
	}
	void Show( sf::RenderWindow &app)
	{
		app.Draw(sprite);
		app.Draw(back); 
	}
private:
	sf::Sprite sprite;
	sf::Sprite back;
	enum{backSpeed = 250};
};

int main()
{
	sf::RenderWindow App(sf::VideoMode(ScreenWidth,ScreenHeight), "Main");
	
	Player player("sprites.png"); 
	Background background("fire.png");
	Player p2("sprites.png"); 
	
	while(App.IsOpened())
	{
		sf::Event event;
		while(App.GetEvent(event))
		{
			if(event.Type == event.Closed)
				App.Close();	
			if(App.GetInput().IsKeyDown(sf::Key::Escape))
				App.Close();
		}
		
		//Sprite events
		player.Move(App);
		p2.Move(App);
		background.Move(App);
		
		App.Clear(sf::Color(100,100,100));
		
		//Sprite displays
		background.Show(App);
		player.Show(App);
		
		//Window draw
		App.Display();								
	}
}