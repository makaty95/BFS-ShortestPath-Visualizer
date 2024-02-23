#include "UIME.h"

/// <summary>
/// Mouses the on.
/// </summary>
/// <param name="shape">The shape.</param>
/// <param name="window">The window.</param>
/// <returns></returns>
bool mouseOn(Shape& shape, RenderWindow& window) {
	if (shape.getGlobalBounds().contains({ (float)Mouse::getPosition(window).x, (float)Mouse::getPosition(window).y })) {
		return 1;
	}
	return 0;
}

//////////////////////////Button////////////////////////////////////////////////////////////
bool UIComponent::alreadyClicked = 0;

//constructor
/// <summary>
/// Initializes a new instance of the <see cref="Button"/> class.
/// </summary>
/// <param name="font">The font.</param>
/// <param name="forColor">For color.</param>
/// <param name="backColor">Color of the back.</param>
/// <param name="position">The position.</param>
/// <param name="textString">The text string.</param>
Button::Button(Font& font, const Color& forColor, const Color& backColor, Vector2f position, string textString) : UIComponent(UIType::BUTTON) {
	onHavor = false;
	text.setScale({ 1.f, 1.f });
	this->setText(textString);
	this->setFont(font);
	this->setTextSize(20);
	this->setBackColor(backColor);

	havor_color = Color(122, 122, 122);
	click_color = Color(122, 12, 13);
	padding = Vector2f(10.f, 10.f);
	
	this->setPosition(position);
	this->setBackColor(backColor);
	this->setForColor(forColor);
	this->autoSetSize();
	// text refactor

}


//other functions
/// <summary>
/// Determines whether [is mouse contained] [the specified window].
/// </summary>
/// <param name="window">The window.</param>
/// <returns>
///   <c>true</c> if [is mouse contained] [the specified window]; otherwise, <c>false</c>.
/// </returns>
bool Button::isMouseContained(RenderWindow& window) {
	if (body.getGlobalBounds().contains({ (float)Mouse::getPosition(window).x, (float)Mouse::getPosition(window).y })) {
		return 1;
	}
	return 0;
}

void Button::behaviour(RenderWindow& window) {
	
	//on mouse havour
	if (!onHavor and isMouseContained(window)) {
		body.setFillColor(havor_color);

		// cursor behaviour
		Cursor cursor;
		cursor.loadFromSystem(Cursor::Hand);
		window.setMouseCursor(cursor);
		onHavor = true;

		
	}
	else if(onHavor and !isMouseContained(window)){
		body.setFillColor(backColor);

		// cursor
		Cursor cursor;
		cursor.loadFromSystem(Cursor::Arrow);
		window.setMouseCursor(cursor);
		onHavor = false;
	}
}

void Button::setOnHavorColor(Color color) {
	this->havor_color = color;
}
void Button::setOnClickColor(Color color) {
	this->click_color = color;
}

void Button::allignText(Allignement allignment) {
	Vector2f txt_dimensions = getTextDimentions();
	Vector2f btn_dimensions = getButtonDimentions();
	Vector2f btn_position = getPosition();
	if (allignment == Allignement::CENTER) {

		// calculating the position to make the text in the center.
		padding = Vector2f((btn_dimensions.x - txt_dimensions.x) / 2.0, (btn_dimensions.y - txt_dimensions.y) / 2.0 - txt_dimensions.y / 2.0);

		// setting the text the the position
		this->text.setPosition(Vector2f(btn_position.x + padding.x, btn_position.y + padding.y));
	}
}
/// <summary>
/// Determines whether the specified window is clicked.
/// </summary>
/// <param name="window">The window.</param>
/// <returns>
///   <c>true</c> if the specified window is clicked; otherwise, <c>false</c>.
/// </returns>
bool Button::isClicked(RenderWindow& window) {
	if (mouseOn(body, window)) {

		if (!alreadyClicked and Mouse::isButtonPressed(Mouse::Left)) {
			
			body.setFillColor(click_color);
			onHavor = 0;
			alreadyClicked = 1;
			return 1;
			
		}
		else if (!Mouse::isButtonPressed(Mouse::Left)) {
			alreadyClicked = 0;
		}
	} 
	return 0;
}

//inside private functions
void Button::autoSetSize() {
	// set the initial size of the body
	body.setSize({ (float)2.0 * padding.x + text.getGlobalBounds().width ,(float)text.getGlobalBounds().height + 2.f * padding.y });
}

/// <summary>
/// Relocates the text.
/// </summary>
void Button::relocateText() {
	// to auto resize the button with the text
	Vector2f btn_pos = this->getPosition(); // position of the button
	Vector2f btn_dimentions = getButtonDimentions();
	Vector2f txt_dimentions = getTextDimentions();
	Vector2f pos;
	pos.x = btn_pos.x + padding.x;
	pos.y = btn_pos.y + padding.y;
	this->text.setPosition(pos);

}

// to be able to draw the button with .draw function
void Button::draw(RenderTarget& _target, RenderStates states) const {
	_target.draw(body, states);
	_target.draw(text, states);
}

//setters
void Button::setBackColor(const Color& color) { this->body.setFillColor(color); this->backColor = color; } //done

void Button::setForColor(const Color& color) { this->text.setFillColor(color); } //done

void Button::setText(string text) { this->text.setString(text); this->autoSetSize(); } //done

void Button::setPosition(Vector2f position) { //done

	this->body.setPosition(position);
	this->text.setPosition(position.x + padding.x, position.y + padding.y);
}
void Button::setFont(Font& font) { this->text.setFont(font); } //done

void Button::setSize(Vector2f size) {
	//to be implemented later

	// if the width and height of the body is enought to center the text
	if (size.x >= getTextDimentions().x and size.y >= getTextDimentions().y) {
		this->body.setSize(size);
		this->relocateText();

	}
	else {
		/*
			we have to resize the font to a smaller 
			one, we could do this later, lets just 
			ignore this for now , and ouptput an alert message.
		*/ 
		cout << "[ALERT!] Can not resize buton, try larger dimentions (still under developement.)\n";
	}
	

} 
void Button::setTextSize(unsigned int size) { this->text.setCharacterSize(size); this->autoSetSize(); } //to be implemented later

//getters
Vector2f Button::getPosition() {
	return this->body.getPosition();
}

Vector2f Button::getTextDimentions() {
	return Vector2f(this->text.getGlobalBounds().width, this->text.getGlobalBounds().height);
}

Vector2f Button::getButtonDimentions() {
	return Vector2f(this->body.getGlobalBounds().width, this->body.getGlobalBounds().height);
}


//////////////////////////Button////////////////////////////////////////////////////////////

//////////////////////////Panel////////////////////////////////////////////////////////////

Panel::Panel(Vector2f _size, const Color& _color, Vector2f _position) : UIComponent(UIType::PANEL) {
	this->setFillColor(_color);
	this->setPosition(_position);
	this->setSize(_size);
}

bool Panel::isMouseContained(RenderWindow& window) {

	if (this->getGlobalBounds().contains({ (float)Mouse::getPosition(window).x, (float)Mouse::getPosition(window).y })) {
		return 1;
	}
	return 0;
}

void Panel::addComponent(UIComponent& _component) {

	if (_component.componentType == UIType::PANEL) {

		Panel& p = static_cast<Panel&>(_component);
		p.setPosition({ this->getPosition().x + p.getPosition().x, this->getPosition().y + p.getPosition().y });

	}else if(_component.componentType == UIType::BUTTON) {

		Button& b = static_cast<Button&>(_component);
		b.setPosition({this->getPosition().x + b.getPosition().x, this->getPosition().y + b.getPosition().y });

	}else if (_component.componentType == UIType::LABEL) {

		Label& ll = static_cast<Label&>(_component);
		ll.setPosition({ this->getPosition().x + ll.getPosition().x, this->getPosition().y + ll.getPosition().y });

	}
	else if (_component.componentType == UIType::CHECKBOX) {
		CheckBox& cb = static_cast<CheckBox&>(_component);

		cb.setPosition({ this->getPosition().x + cb.getPosition().x, this->getPosition().y + cb.getPosition().y });

	}
	
}

//////////////////////////Panel////////////////////////////////////////////////////////////

//////////////////////////Label////////////////////////////////////////////////////////////

// constructor
Label::Label(Font& font, Vector2f _position, string textString) : UIComponent(UIType::LABEL) {
	this->setFont(font);
	this->setPosition(_position);
	this->setCharacterSize(20);
	this->setString(textString);
}

void Label::setValue(int value) {
	this->setString(to_string(value));
}

bool Label::isMouseContained(RenderWindow& window) {

	if (this->getGlobalBounds().contains({ (float)Mouse::getPosition(window).x, (float)Mouse::getPosition(window).y })) {
		return 1;
	}
	return 0;
}

//////////////////////////////////////////////////////Label////////////////////////////////////////////////////////////

///////////////////////////////////////////////////CheckBox////////////////////////////////////////////////////////////

void CheckBox::draw(RenderTarget& _target, RenderStates states) const {
	_target.draw(text, states);
	_target.draw(layOut, states);
	_target.draw(checkDot, states);

}

void CheckBox::autoRefactorPosition(const Vector2f& oldPosition) {
	this->layOut.setPosition(oldPosition);
	this->checkDot.setPosition(Vector2f(oldPosition.x + (this->layOut.getGlobalBounds().width / 4.0),
		oldPosition.y + (this->layOut.getGlobalBounds().height / 4.0)));
	this->text.setPosition(Vector2f(oldPosition.x + this->layOut.getGlobalBounds().width + 4.0, oldPosition.y));

}

// static variables initialization
float CheckBox::default_layOutLength = 15.f;
float CheckBox::default_textSize = 14.f;

void CheckBox::setScale(const float& scale) {
	Vector2f currentPosition = layOut.getPosition();
	this->layOut.setSize(Vector2f(default_layOutLength*scale, default_layOutLength*scale));
	this->checkDot.setSize(Vector2f(default_layOutLength * scale / 2.0, default_layOutLength * scale / 2.0));
	this->text.setCharacterSize(default_textSize*scale);

	this->autoRefactorPosition(currentPosition);
}

void CheckBox::setPosition(const Vector2f& position) {
	this->layOut.setPosition(position);

	this->checkDot.setPosition(Vector2f(position.x + (layOut.getGlobalBounds().width / 4.0),
		position.y + (layOut.getGlobalBounds().height / 4.0)));

	this->text.setPosition(Vector2f(position.x + this->layOut.getGlobalBounds().width + 4.0, position.y));


}

CheckBox::CheckBox(String text, Font& textFont, Vector2f position, bool isChecked) : UIComponent(UIType::CHECKBOX){
	this->setChecked(isChecked);
	this->checkDot.setFillColor(Color::Black);
	this->layOut.setFillColor(Color::White);

	//defaults
	onHavor = 0;
	is_checked = 1;

	// initial size
	this->layOut.setSize(Vector2f(default_layOutLength, default_layOutLength));
	this->checkDot.setSize(Vector2f(default_layOutLength/2.0, default_layOutLength/2.0));


	// initial position
	this->setPosition(position);

	// text common stuff
	this->text.setCharacterSize(default_textSize);
	this->text.setFillColor(Color::Black);
	this->text.setString(text);
	this->text.setFont(textFont);
	

}

bool CheckBox::isChecked() const{
	return this->is_checked;
}
void CheckBox::setChecked(const bool& status) {
	if (status == false) {
		checkDot.setScale(Vector2f(0, 0));
	}
	else {
		checkDot.setScale(Vector2f(1.0, 1.0));
	}
	is_checked = !is_checked;
}

bool CheckBox::isClicked(RenderWindow& window) {
	if (mouseOn(layOut, window)) {

		if (!alreadyClicked and Mouse::isButtonPressed(Mouse::Left)) {

			// if clicked
			this->setChecked(!is_checked);

			alreadyClicked = 1;
			return 1;

		}
		else if (!Mouse::isButtonPressed(Mouse::Left)) {
			alreadyClicked = 0;
		}
	}
	return 0;
}

// behaviour of the checkBox.
void CheckBox::behaviour(RenderWindow& window) {
	
	if (!onHavor and mouseOn(layOut, window)) {

			// cursor behaviour
			Cursor cursor;
			cursor.loadFromSystem(Cursor::Hand);
			window.setMouseCursor(cursor);
			onHavor = true;
	}
	else if (onHavor and !mouseOn(layOut, window)) {

		// cursor
		Cursor cursor;
		cursor.loadFromSystem(Cursor::Arrow);
		window.setMouseCursor(cursor);
		onHavor = false;
	}
	
}


Vector2f CheckBox::getPosition() const {
	return this->layOut.getPosition();
}