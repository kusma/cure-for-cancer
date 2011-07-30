class Menu{
	bool sound;
	bool fullscreen;
	int bpp;
private:
	Menu();
public:
	inline static Menu* instance(){
		static Menu instance;
		return &instance;
	};
	//~Menu();
	void SetFullscreen( bool fullscreen ){ this->fullscreen = fullscreen; };
	bool Fullscreen(){ return fullscreen; };

	void SetSound( bool sound ){ this->sound = sound; };
	bool Sound(){ return sound; };

	void SetBpp( int bpp ){ this->bpp = bpp; };
	int Bpp(){ return bpp; };
};
