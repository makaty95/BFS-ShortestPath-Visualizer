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

	/////////////////////////////////////////////////////
	/// \brief a button class constructor.
	/// 
	/// \param font: The text font.
	/// \param forColor: The text color.
	/// \param backColor: the button color.
	/// \param position: the button position.
	/// \param textString: the text of the button.
	/////////////////////////////////////////////////////
	Button(Font& font, const Color& forColor = Color::Black, const Color& backColor = Color::White,Vector2f position = { 0.f, 0.f }, string textString = "");

	//setters
	void setBackColor(const Color& color);
	void setForColor(const Color& color);
	Vector2f getButtonDimentions();
	Vector2f getTextDimentions();
	void setText(string text);
		
	

	///////////////////////////////////////////////////////////////////////////////////////////
	/// \brief change the button position. Note that if you added it to a panel,
	/// then the position is calculated from the panel topLeft corner.
	/// 
	/// \param position: the desired position.
	////////////////////////////////////////////////////////////////////////////////////////////
	void setPosition(Vector2f position);
	
	
	///////////////////////////////////////////////////////////////////////////////////////////
	/// \brief change the button font.
	/// 
	/// \param font: the desired font.
	////////////////////////////////////////////////////////////////////////////////////////////
	void setFont(Font& font);

	///////////////////////////////////////////////////////////////////////////////////////////
	/// \brief changes the button size. 
	///
	/// \param size: the desired size.
	///////////////////////////////////////////////////////////////////////////////////////////
	void setSize(Vector2f size); 
	
	
	///////////////////////////////////////////////////////////////////////////////////////////
	/// \brief change the text size.
	/// 
	/// \param size: the desired size.
	////////////////////////////////////////////////////////////////////////////////////////////
	void setTextSize(unsigned int size); // under review
	

	///////////////////////////////////////////////////////////////////////////////////////////
	/// \brief change the color when the button is havored.
	/// 
	/// \param color the desired color.
	////////////////////////////////////////////////////////////////////////////////////////////
	void setOnHavorColor(Color color);
	
	///////////////////////////////////////////////////////////////////////////////////////////
	/// \brief change the color when the button is clicked.
	/// 
	/// \param color the desired color.
	////////////////////////////////////////////////////////////////////////////////////////////
	void setOnClickColor(Color color);



	///////////////////////////////////////////////////////////////////////////////////////////
	/// \brief allign the button text to left, right or center.
	/// 
	/// \param allignment the allignment of the text.
	////////////////////////////////////////////////////////////////////////////////////////////
	void allignText(Allignement allignement); //later


	//getters
	///////////////////////////////////////////////////////////////////////////////////////////
	/// \brief get the position of the button.
	///
	////////////////////////////////////////////////////////////////////////////////////////////
	Vector2f getPosition();

	//others
	///////////////////////////////////////////////////////////////////////////////////////////
	/// \brief active the button behaviours
	/// 
	///  like havor, click colors
	///
	/// \param window the RenderWindow.
	///////////////////////////////////////////////////////////////////////////////////////////
	void behaviour(RenderWindow& window);

	///////////////////////////////////////////////////////////////////////////////////////////
	/// \brief check if the button clicked by the left mouse button.
	/// 
	/// \param window the RenderWindow.
	///////////////////////////////////////////////////////////////////////////////////////////
	virtual bool isClicked(RenderWindow& window);
};


/// \brief simple UI Panel class that can contain items like: buttons, panels and labels.
/// 
/// \param _size The panel size.
/// \param _color The panel backColor.
/// \param _position the panel poistion.
class Panel : public RectangleShape, public UIComponent {
	
public:
	///////////////////////////////////////////////////////////////////////////////////////////
	/// \brief add a UIComponent to the panel body. Note that if you added a component
	/// to the panel, then its coordinates is determined from the panel topLeft corner.
	/// \param _component the component to add to the panel,
	/// can be : [button, label or panel]
	////////////////////////////////////////////////////////////////////////////////////////////
	void addComponent(UIComponent& _component);


	///////////////////////////////////////////////////////////////////////////////////////////
	/// \brief check if mouse is inside the panel.
	/// \param window the RenderWindow.
	////////////////////////////////////////////////////////////////////////////////////////////
	bool isMouseContained(RenderWindow& window);

	///////////////////////////////////////////////////////////////////////////////////////////
	/// \brief panel class constructor.
	/// \param _size The panel size.
	/// \param _color The panel backColor.
	/// \param _position the panel poistion.
	////////////////////////////////////////////////////////////////////////////////////////////
	Panel(Vector2f _size, const Color& _color = Color::White, Vector2f _position = { 0.f, 0.f });

};




/// \brief simple label class.
/// \param font the text font.
/// \param _position the position of the label.
/// \param textString the text of the label.
class Label : public Text, public UIComponent {
public:
	/////////////////////////////////////////////////
	/// \brief label class constructor.
	/// \param font the text font.
	/// \param _position the position of the label.
	/// \param textString the text of the label.
	/////////////////////////////////////////////////
	Label(Font& font, Vector2f _position = { 0.f, 0.f }, string textString = " ");
	
	/////////////////////////////////////////////////
	/// \brief check if the mouse is inside the label
	/// \param font the text font.
	/// \param _position the position of the label.
	/// \param textString the text of the label.
	/////////////////////////////////////////////////
	bool isMouseContained(RenderWindow& window);

	/////////////////////////////////////////////////
	/// \brief set the label text with an integer.
	/// \param value the integer value.
	/////////////////////////////////////////////////
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
