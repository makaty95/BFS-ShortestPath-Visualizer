#pragma once
#include<iostream>
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"
using namespace sf;
using namespace std;

//////////////////////////////Utility////////////////////////////////////////////////////////////////////////
bool mouseOn(Shape& shape, RenderWindow& window);
enum class UIType{ BUTTON, PANEL, LABEL , CHECKBOX, NONE};
enum class Allignement { CENTER, LEFT, RIGHT };


//////////////////////////////Utility////////////////////////////////////////////////////////////////////////


class UIComponent {

protected:
	static bool alreadyClicked;
	bool onHavor;

public:
	UIType componentType;

	UIComponent(UIType _type = UIType::NONE) { this->componentType = _type; }
	virtual bool isClicked(RenderWindow& window) { return 0; }
	virtual bool isMouseContained(RenderWindow& window) {return 0;}
};


////////////////////////////////////////////////////

/////////////////////////////////////////////////////
/// \brief simple UI Button class.
/// 
/// \param font The text font.
/// 
/// \param forColor The text color.
/// 
/// \param backColor the button color.
/// 
/// \param position the button poistion.
/// 
/// \param textString the text of the button.
/// 
//////////////////////////////////////////////////////
class Button : public Drawable, public UIComponent {
private:		

	Vector2f padding;
	Color backColor;
	Text text;
	void autoSetSize();
	void relocateText();
	virtual void draw(RenderTarget& _target, RenderStates states) const override;
	bool isMouseContained(RenderWindow& window);
protected:


public:


	Color havor_color;
	Color click_color;
	RectangleShape body;

	Button(Font& font, const Color& forColor = Color::Black, const Color& backColor = Color::White,Vector2f position = { 0.f, 0.f }, string textString = "");

	//getters
	Vector2f getButtonDimentions();

	Vector2f getPosition();

	Vector2f getTextDimentions();

	//setters
	void setBackColor(const Color& color);

	void setText(string text);

	void setForColor(const Color& color);

	void setPosition(Vector2f position);
	
	void setFont(Font& font);

	void setSize(Vector2f size); 
	
	void setTextSize(unsigned int size); // under review
	
	void setOnHavorColor(Color color);
	
	void setOnClickColor(Color color);

	void allignText(Allignement allignement); //later



	//others
	void behaviour(RenderWindow& window);

	virtual bool isClicked(RenderWindow& window);
};


/// \brief simple UI Panel class that can contain items like: buttons, panels and labels.
/// 
/// \param _size The panel size.
/// \param _color The panel backColor.
/// \param _position the panel poistion.
class Panel : public RectangleShape, public UIComponent {
	
public:

	void addComponent(UIComponent& _component);


	bool isMouseContained(RenderWindow& window);


	Panel(Vector2f _size, const Color& _color = Color::White, Vector2f _position = { 0.f, 0.f });

};




/// \brief simple label class.
/// \param font the text font.
/// \param _position the position of the label.
/// \param textString the text of the label.
class Label : public Text, public UIComponent {
public:

	Label(Font& font, Vector2f _position = { 0.f, 0.f }, string textString = " ");
	

	bool isMouseContained(RenderWindow& window);


	void setValue(int value);
};

class CheckBox : public Drawable, public UIComponent {
private:
	RectangleShape checkDot, layOut;
	Text text;
	bool is_checked;
	static float default_layOutLength;
	static float default_textSize;
	virtual void draw(RenderTarget& _target, const RenderStates states) const override;
	void autoRefactorPosition(const Vector2f& oldPosition);

protected:

public:
	CheckBox(String text, Font& textFont, Vector2f position = {0.f, 0.f}, bool isChecked = 1);
	

	// getters
	virtual bool isClicked(RenderWindow& window);
	bool isChecked() const;
	Vector2f getPosition() const;


	// setters
	void setChecked(const bool& status);
	void setScale(const float& scale);
	void setPosition(const Vector2f& position);

	void behaviour(RenderWindow& window);
};
