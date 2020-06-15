#include <SFML/Graphics.hpp>
#include "parser/mapPar.h"
#include "view.h"
#include <iostream>
#include <sstream>
//#include "mission.h"
#include "iostream"
//#include "map.h"
#include <vector>
#include <list>
 
//#include "csv.h"
 
using namespace sf;
////////////////////////////////////����� �����-��������//////////////////////////
class Entity {
public:
	std::vector<Object> obj;//������ �������� �����
	float dx, dy, x, y, speed,moveTimer;
	int w,h,health;
	bool life, isMove, onGround;
	Texture texture;
	Sprite sprite;
	String name;
	Entity(Image &image, String Name, float X, float Y, int W, int H)
	{
		x = X; y = Y; w = W; h = H; name = Name; moveTimer = 0;
		speed = 0; health = 100; dx = 0; dy = 0;
		life = true; onGround = false; isMove = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
	}
 
	FloatRect getRect()
	{//�-��� ��������� ��������������. ��� �����,������� (���,�����).
		return FloatRect(x, y, w, h);//��� �-��� ����� ��� �������� ������������ 
	}
};
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////�������/////////////////////////////
class winTrigger :public Entity {
public:
	bool win;
	winTrigger(Image& image, String Name, TileMap& lev, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H)
	{
		win = false; obj = lev.getObjectsByName("winTrigger");
	}

	void triggerWin(float Dx, float Dy)
	{
		Text winT;
		Texture wwinI;
		Sprite winS;

		for (int i = 0; i < obj.size(); i++)
			if (getRect().intersects(obj[i].rect))
			{
				if (obj[i].name == "winTrigger")
				{
					if (Dy > 0 && Dy < 0 && Dx>0 && Dx < 0) {
						winT.setString("Goal!");
						winT.setPosition(520, 430);
						std::cout << "aaa";
					}
				}
			}
	}
	void update(float time)
	{

	}
};
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////����� ������////////////////////////
class Player :public Entity {
public:
	enum { left, right, up, down, jump, stay } state;
	int playerScore;
	Player(Image& image, String Name, TileMap& lev, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		playerScore = 0; state = stay; obj = lev.getAllObjects();//��������������.�������� ��� ������� ��� �������������� ��������� � ������
		if (name == "Player1") {
			sprite.setTextureRect(IntRect(4, 19, w, h));
		}
	}

	void control() 
	{
		if (Keyboard::isKeyPressed) {
			if (Keyboard::isKeyPressed(Keyboard::A)) {
				state = left; 
				speed = 0.1;
				if (Keyboard::isKeyPressed(Keyboard::LShift)) speed = 0.3;
			}
			if (Keyboard::isKeyPressed(Keyboard::D)) {
				state = right;
				speed = 0.1;
				if (Keyboard::isKeyPressed(Keyboard::LShift)) speed = 0.3;

			}

			if ((Keyboard::isKeyPressed(Keyboard::Space)) && (onGround)) {
				state = jump; dy = -0.6; onGround = false;
			}

			if (Keyboard::isKeyPressed(Keyboard::S)) {
				state = down;
			}
			
			
		}

		//std::cout << speed << "\n";
	}
 

 
	   void checkCollisionWithMap(float Dx, float Dy)
	   {

		   for (int i = 0; i<obj.size(); i++)//���������� �� ��������
		   if (getRect().intersects(obj[i].rect))//��������� ����������� ������ � ��������
		   {
			   if (obj[i].name == "solid")//���� ��������� �����������
			   {
				   if (Dy>0)	{ y = obj[i].rect.top - h;  dy = 0; onGround = true; } //����� 
				   if (Dy<0)	{ y = obj[i].rect.top + obj[i].rect.height;   dy = 0; } //����
				   if (Dx>0)	{ x = obj[i].rect.left - w; }// �����...
				   if (Dx<0)	{ x = obj[i].rect.left + obj[i].rect.width; } //������...
			   }
			   if (obj[i].name == "box")//���� ��������� �����������
			   {
				   if (Dy > 0) { y = obj[i].rect.top - h;  dy = 0; onGround = true; }
				   //if (Dy < 0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
				   //if (Dx > 0) { x = obj[i].rect.left - w; }
				   //if (Dx < 0) { x = obj[i].rect.left + obj[i].rect.width; }
			   }
		   }
	   }
	    
	   void update(float time)
	   {
		   control();
		   switch (state)
		   {
		   case right:dx = speed; break;
		   case left:dx = -speed; break;
		   case up: break;
		   case down: dx = 0; break;
		   case stay: break;
		   }
		   x += dx*time;
		   checkCollisionWithMap(dx, 0);
		   
		   y += dy*time;
		   checkCollisionWithMap(0, dy);
		   
		   sprite.setPosition(x + w / 2, y + h / 2);
		   if (health <= 0){ life = false; }
		   if (!isMove){ speed = 0; }
		   setPlayerCoordinateForView(x, y);
		   if (life) { setPlayerCoordinateForView(x, y); }
		   dy = dy + 0.0015*time;
	   } 
};
 ////////////////////////////////////////////////////////////////////////////////////
 
 ///////////////////////////////////����� �����//////////////////////////////////////
class Enemy :public Entity{
public:
	Enemy(Image &image, String Name, TileMap& lev, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H){
		obj = lev.getObjectsByName("solid");//��������������.�������� ������ ������� ��� �������������� ����� � ������
		if (name == "EnemySpawn"){
			sprite.setTextureRect(IntRect(0, 0, w, h));
			dx = 0.1;
		}
	}
 
	void checkCollisionWithMap(float Dx, float Dy)
	{
		
		for (int i = 0; i<obj.size(); i++)//���������� �� ��������
		if (getRect().intersects(obj[i].rect))//��������� ����������� ������ � ��������
		{
			if (obj[i].name == "solid"){//���� ��������� ����������� (������ � ������ solid)
				if (Dy>0)	{ y = obj[i].rect.top - h;  dy = 0; onGround = true; }
				if (Dy<0)	{ y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
				if (Dx>0)	{ x = obj[i].rect.left - w;  dx = -0.1; sprite.scale(-1, 1); }
				if (Dx<0)	{ x = obj[i].rect.left + obj[i].rect.width; dx = 0.1; sprite.scale(-1, 1); }
			}
		}
	}
 
	void update(float time)
	{
		if (name == "EnemySpawn"){
			//moveTimer += time;if (moveTimer>3000){ dx *= -1; moveTimer = 0; }//������ ����������� �������� ������ 3 ���
			checkCollisionWithMap(dx, 0);
			x += dx*time;
			sprite.setPosition(x + w / 2, y + h / 2);
			if (health <= 0){ life = false; }
		}
	}
};
//////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////�������� ������� main//////////////////////////////////
int main()
{
	RenderWindow window(VideoMode(1040, 860), "window");
	view.reset(FloatRect(0, 0, 640, 480));
	TileMap lvl;
	//������� ��������� ������ �������
	lvl.load("D:/Users/Public/source/repos/test 2/x64/Release/map/NewTile5.tmx");//��������� � ���� �����, ������ ������ � ������� ������� �� �� ����������.

	Image heroImage;
	heroImage.loadFromFile("D:/Users/Public/source/repos/test 2/x64/Release/textures/MilesTailsPrower.gif");
	Texture textUre;
	textUre.loadFromImage(heroImage);
 
	Image easyEnemyImage;
	easyEnemyImage.loadFromFile("D:/Users/Public/source/repos/test 2/x64/Release/textures/link.png");
	easyEnemyImage.createMaskFromColor(Color(0, 64, 64));

	Image TriggerImage1;
	TriggerImage1.loadFromFile("D:/Users/Public/source/repos/test 2/x64/Release/textures/win.png");
 
	Object player=lvl.getObject("player");//������ ������ �� ����� �����.������ ���������� ������ � ������ ��� ������ ����
	Object easyEnemyObject = lvl.getObject("EnemySpawn");//������ ������� ����� �� ����� �����.������ ���������� ������ � ������ ��� ������ ����
	Object TriggerObject = lvl.getObject("winTrigger"); //win trigger
 
	Player p(heroImage, "Player1", lvl, player.rect.left, player.rect.top, 40, 30);//�������� ���������� �������������� player �� ����� � ���������� ������ ������
	Enemy easyEnemy(easyEnemyImage, "EnemySpawn", lvl, easyEnemyObject.rect.left, easyEnemyObject.rect.top, 60, 60);//�������� ���������� �������������� easyEnemy �� ����� � ���������� ������ �����
	winTrigger Trigger(TriggerImage1, "TriggerWin", lvl, TriggerObject.rect.left, TriggerObject.rect.left, 32, 32);

	Clock clock;
	while (window.isOpen())
	{
 
		float time = clock.getElapsedTime().asMicroseconds();
 
		clock.restart();
		time = time / 800;
		
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();	
		}	

		p.update(time);
		easyEnemy.update(time);
		window.setView(view);
		window.clear(Color(77,83,140));

		window.draw(lvl);//������ �����
		window.draw(Trigger.sprite);
		window.draw(easyEnemy.sprite);
		window.draw(p.sprite);

		window.display();
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////