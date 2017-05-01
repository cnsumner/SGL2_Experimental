#include "sgl2.hpp"

using namespace sgl2;

class MyObjectWindow : public ObjectWindow {
private:
	Rectangle* button_;
	Circle* ball_;
	Rectangle* paddle_;
	vector<Rectangle*> bricks_;

	int button_id_;

	bool game_running_;

	float x_;
	float y_;
	float v_x_;
	float v_y_;

public:
	MyObjectWindow(const string& title, int width, int height,
		const Color& background)
		: ObjectWindow(title, width, height, background), button_(nullptr), ball_(nullptr), paddle_(nullptr), game_running_(false), x_(390.0f), y_(290.0f), v_x_(-0.05f), v_y_(0.05f) {
		button_ = new Rectangle(this, Color(0, 255, 0), 0, 0, 100, 30, true);
		ball_ = new Circle(this, Color(255, 100, 0), 390, 290, 9, true);
		paddle_ = new Rectangle(this, Color(255, 255, 255), 750, 500, 150, 20, true);

		button_id_ = button_->id_;

		Add(button_);
		Add(ball_);
		Add(paddle_);

		bricks_ = vector<Rectangle*>();

		for (int y = 0; y < 8; ++y) {
			for (int x = 0; x < (10 - (y % 2 == 0 ? 2 : 0)); ++x) {
				Rectangle* brick = new Rectangle(this, Color(40 * (y + 1), 50 * (x + 1), 0), 80 * (x + (y % 2 == 0 ? 1 : 0)), 20 * y, 80, 20, true);
				bricks_.push_back(brick);
			}
		}
	}

	void StartGame() {
		for (auto b : bricks_)
			Add(b);
		game_running_ = true;
	}

	void MouseMoved(const MouseMotionEvent& event) override {
		paddle_->SetPosX(event.x - paddle_->GetWidth() / 2);
	}

	void MousePressed(const MouseButtonEvent& event) override {
		GraphicalObject* obj = GetFirstHit(event.x, event.y);
		if (obj && obj->id_ == button_id_) {
			Remove(obj);
			StartGame();
		}

	}

	void KeyPressed(const KeyboardEvent& event) override {
		if (event.keysym.sym == SDLK_q)
			Quit();
	}

	void Update() override {
		if (game_running_) {
			ball_->SetPosX(int(x_ += v_x_));
			ball_->SetPosY(int(y_ += v_y_));
			if (ball_->Collision(paddle_)) {
				v_y_ *= -1.0;
				v_x_ += fmodf((float)rand() / RAND_MAX, 0.04f) - 0.02f;
			}

			if (ball_->GetPosY() <= 0)
				v_y_ *= -1.0;

			if (ball_->GetPosX() <= 0 || ball_->GetPosX() >= 789)
				v_x_ *= -1.0;

			if (ball_->GetPosY() > 600) {
				SetBackgroundColor(Color(255, 0, 0));
				Remove(ball_);
				game_running_ = false;
			}

			for (auto brick : bricks_) {
				if (ball_->Collision(brick)) {
					v_x_ *= (x_ + ball_->GetWidth() <= brick->GetPosX() || x_ >= brick->GetPosX() + brick->GetWidth() ? -1.0f : 1.0f);
					v_y_ *= (y_ + ball_->GetHeight() <= brick->GetPosY() || y_ >= brick->GetPosY() + brick->GetHeight() ? -1.0f : 1.0f);
					v_x_ += fmodf((float)rand() / RAND_MAX, 0.04f) - 0.02f;
					Remove(brick);
					bricks_.erase(std::remove(bricks_.begin(), bricks_.end(), brick), bricks_.end());
					break;
				}
			}

			if (bricks_.size() == 0) {
				SetBackgroundColor(Color(0, 255, 0));
				game_running_ = false;
			}

			Repaint();
		}
	}
};

int main() {
	MyObjectWindow window = MyObjectWindow("test", 800, 600, Color(0, 0, 0));
	srand(SDL_GetTicks());

	window.Run();

	return 0;
}