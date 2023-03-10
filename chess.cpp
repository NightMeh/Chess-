#include "Chess.h"
Chess::Chess(const ChessConstructorArgs& args) :
	screenHeight{ args.screenHeight },
	screenWidth{ args.screenWidth },
	texturePath{ args.texturePath }
{
	board.Board::Board({ "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2", true, texturePath });
	boardImg.loadFromFile(texturePath + "/board.png");
	boardSprite.setTexture(boardImg);
	pieceOnMouse = empty;

}
void Chess::createWindow() {
	window.create(sf::VideoMode(screenWidth, screenHeight),"Chess-");
}
void Chess::destroyWindow() {
	window.close();
}

void Chess::drawBoard() {
	if (!board.getDrawBoardOnScreenEnabled()) {
		return;
	}
	window.draw(boardSprite);
	for (auto& piece : board.getPieceSprites()) {
		window.draw(piece.getSprite());
	}
}

void Chess::update() {
	
	while (window.pollEvent(event))
	{
		switch (event.type) {
		case sf::Event::Closed:
			window.close();
			break;
		case sf::Event::KeyPressed:
			switch (event.key.code){
			case sf::Keyboard::Delete:
				deleteAtIndexFromPosition();
				break;
			}
		case sf::Event::MouseButtonPressed:
			switch (event.mouseButton.button) {
			case sf::Mouse::Left:
				pickUpPiece();
				break;
			}
			break;
		case sf::Event::MouseButtonReleased:
			switch (event.mouseButton.button) {
			case sf::Mouse::Left:
				dropPiece();
				break;
			}
		}
	}

	window.clear();
	board.updatePieceSprites();
	drawBoard();
	window.display();

}

void Chess::start() {
	board.updatePieceSprites();
	while (window.isOpen()) {
		update();
	}
	destroyWindow();
}

unsigned int Chess::findIndexFromOnScreenPosition() {
	sf::Vector2i pos = sf::Mouse::getPosition(window);
	int x = pos.x;
	int y = pos.y;
	if (x < 0 || y < 0) {
		return 999;
	}

	x /= 60;
	y /= 60;

	if (x > 7 || y > 7) {
		return 999;
	}
	unsigned int index = x + 8 * y;
	return index;
}

void Chess::deleteAtIndexFromPosition() {
	unsigned int index = findIndexFromOnScreenPosition();
	std::cout << index;
	if (index == 999) {
		std::cout << "return";
		return;
	}
	board.deleteAtIndex(findIndexFromOnScreenPosition());
}

void Chess::pickUpPiece() {
	if (pieceOnMouse != empty) {
		return;
	}
	unsigned int index = findIndexFromOnScreenPosition();
	if (index == 999) {
		return;
	}
	pieceOnMouse = board.pieceAtIndex(index);
	board.deleteAtIndex(index);
}

void Chess::dropPiece() {
	if (pieceOnMouse == empty) {
		return;
	}
	unsigned int index = findIndexFromOnScreenPosition();
	if (index == 999) {
		return;
	}
	board.addPieceToIndex(index, pieceOnMouse);
	pieceOnMouse = empty;
}
