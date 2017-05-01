#include "sgl2.hpp"

using namespace sgl2;

class MyObjectWindow : public ObjectWindow {
private:
	Rectangle* ball_;
	Rectangle* paddle_;
	vector<Rectangle*> bricks_;

	float x_;
	float y_;
	float v_x_;
	float v_y_;

public:
	MyObjectWindow(const string& title, int width, int height,
		const Color& background)
		: ObjectWindow(title, width, height, background), ball_(nullptr), x_(390.0f), y_(290.0f), v_x_(-0.05f), v_y_(0.05f) {
		ball_ = new Rectangle(this, Color(0, 100, 255), 390, 290, 10, 10);
		paddle_ = new Rectangle(this, Color(255, 255, 255), 750, 500, 150, 10);

		Add(ball_);
		Add(paddle_);

		bricks_ = vector<Rectangle*>();

		for (int y = 0; y < 8; ++y) {
			for (int x = 0; x < (10 - (y % 2 == 0 ? 2 : 0)); ++x) {
				Rectangle* brick = new Rectangle(this, Color(40 * (y + 1), 50 * (x + 1), 0), 80 * (x + (y % 2 == 0 ? 1 : 0)), 20 * y, 80, 20);
				bricks_.push_back(brick);
				Add(brick);
			}
		}
	}

	void MouseMoved(const MouseMotionEvent& event) override {
		paddle_->SetPosX(event.x - paddle_->GetWidth()/2);
	}

	void KeyPressed(const KeyboardEvent& event) override {
		if (event.keysym.sym == SDLK_q)
			Quit();
	}

	void Update() override {
		ball_->SetPosX(int(x_ += v_x_));
		ball_->SetPosY(int(y_ += v_y_));
		if (ball_->Collision(paddle_)) {
			v_y_ *= -1.0;
			v_x_ += fmodf((double)rand() / RAND_MAX, 0.04) - 0.02;
		}

		if (ball_->GetPosY() <= 0)
			v_y_ *= -1.0;

		if (ball_->GetPosX() <= 0 || ball_->GetPosX() >= 789)
			v_x_ *= -1.0;

		if (ball_->GetPosY() > 600)
			SetBackgroundColor(Color(255, 0, 0));

		for (auto brick : bricks_) {
			if (ball_->Collision(brick)) {
				v_x_ *= (x_ + ball_->GetWidth() <= brick->GetPosX() || x_ >= brick->GetPosX() + brick->GetWidth() ? -1.0 : 1.0);
				v_y_ *= (y_ + ball_->GetHeight() <= brick->GetPosY() || y_ >= brick->GetPosY() + brick->GetHeight() ? -1.0 : 1.0);
				v_x_ += fmodf((double) rand() / RAND_MAX, 0.04) - 0.02;
				Remove(brick);
				bricks_.erase(std::remove(bricks_.begin(), bricks_.end(), brick), bricks_.end());
				break;
			}
		}

		Repaint();
	}
};

int main() {
	MyObjectWindow window = MyObjectWindow("test", 800, 600, Color(0, 0, 0));
	srand(SDL_GetTicks());

	window.Run();

	return 0;
}