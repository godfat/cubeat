
/* 2007.11.9
   Too many things infected by Virtools SDK things,
   needs total overhual, but the structure might possibly remain
   the same, so I'll just keep most of the things unchanged for future references.

   Basically this file is deprecated and cannot compile.
*/

#ifndef _SHOOTING_CUBE_MC_HPP_
#define _SHOOTING_CUBE_MC_HPP_

#include "Location.hpp"
#include "Debugger.hpp"

#include "Sound.hpp"

#include <boost/function.hpp>
#include <loki/Singleton.h>

#include <vector>

class MC{
public:
    MC(): parent_(NULL){}
    MC& x(int x){ loc_.x = x; return adjust_pos(); }
    MC& y(int y){ loc_.y = y; return adjust_pos(); }
    MC& z(int z){ loc_.z = z; return adjust_pos(); }
    MC& xyz(int x, int y, int z){ loc_.x = x; loc_.y = y; loc_.z = z; return adjust_pos(); }
    MC& show(){ body_->Show(CKSHOW); return *this; }
    MC& hide(){ body_->Show(CKHIDE); return *this; }
    MC& parent(CK3dEntity* parent){ parent_ = parent; return *this; }
    bool is_you(CK3dEntity const* picked) const{ return body_ == picked; }
    CK3dEntity*& body(){ return body_; }
    CK3dEntity const* body() const{ return body_; }
    ~MC(){
        context->DestroyObject(body_, CK_DESTROY_TEMPOBJECT);
    }

private:
    MC& adjust_pos(){
        VxVector vloc(loc_.x, loc_.y, loc_.z);
        body_->SetPosition(&vloc, parent_);
        return *this;
    }

private:
    Location loc_;
    CK3dEntity* body_;
    CK3dEntity* parent_;
};


class Button
{
public:
    template <class Callback>
    Button(char const* to_be_loaded, Callback callback): callback_(callback){
        CKMaterial* mat;
        CKTexture* tex = static_cast<CKTexture*>(context->CreateObject(CKCID_TEXTURE, 0, CK_OBJECTCREATION_DYNAMIC));
        VxColor col(150, 150, 150);
        CKDependencies d;
        d.m_Flags = CK_DEPENDENCIES_FULL;
        body_.body() = static_cast<CK3dEntity*>(context->CopyObject(menu_frame, &d, 0, CK_OBJECTCREATION_DYNAMIC));
		body_.body()->SetScale(1.0f, 1.0f, 0.5f);

        tex->LoadImage(const_cast<char*>(to_be_loaded));
        mat = body_.body()->GetMesh(0)->GetMaterial(0);
        // Debugger::i().trace("begin making button");
        mat->SetTexture(tex);
	    mat->EnableAlphaBlend();
        mat->SetSourceBlend(VXBLEND_SRCALPHA);
	    mat->SetDestBlend(VXBLEND_INVSRCALPHA);
	    mat->SetTextureMinMode(VXTEXTUREFILTER_NEAREST);
        mat->SetTextureMagMode(VXTEXTUREFILTER_LINEAR);
        mat->SetTextureAddressMode(VXTEXTURE_ADDRESSWRAP);
        mat->SetTextureBlendMode(VXTEXTUREBLEND_MODULATEALPHA);
        mat->SetDiffuse(col);
        context->GetCurrentLevel()->AddObject(body_.body());
        body_.show();

		focused_ = false;
    }

    Button& x(int x){ body_.x(x); return *this; }
    Button& y(int y){ body_.y(y); return *this; }
    Button& z(int z){ body_.z(z); return *this; }
    Button& xyz(int x, int y, int z){ body_.xyz(x, y, z); return *this; }
    Button& parent(CK3dEntity* parent){ body_.parent(parent); return *this; }
    Button& show(){ body_.show(); return *this; }
    Button& hide(){ body_.hide(); return *this; }

    Button& leave_focus(){
        body_.body()->GetMesh(0)->GetMaterial(0)->SetEmissive(VxColor(128, 128, 128));
        return *this;
    }

    Button& on_focus(){
        body_.body()->GetMesh(0)->GetMaterial(0)->SetEmissive(VxColor(255, 255, 255));
        return *this;
    }

    Button& on_click(){
		Sound::i().proceed();
        callback_();
        return *this;
    }

    bool is_you(CK3dEntity const* picked) const{
        return body_.is_you(picked);
    }

private:
    boost::function<void()> callback_;
    MC body_;
	bool focused_;
};

class Title
{
public:
    Title(char const* to_be_loaded)
	{
        CKMaterial* mat;
        CKTexture* tex = static_cast<CKTexture*>(context->CreateObject(CKCID_TEXTURE, 0, CK_OBJECTCREATION_DYNAMIC));
        VxColor col(255, 255, 255);
        CKDependencies d;
        d.m_Flags = CK_DEPENDENCIES_FULL;
        body_.body() = static_cast<CK3dEntity*>(context->CopyObject(menu_frame, &d, 0, CK_OBJECTCREATION_DYNAMIC));
		body_.body()->SetScale(2.0f, 1.0f, 2.0f);

        tex->LoadImage(const_cast<char*>(to_be_loaded));
        mat = body_.body()->GetMesh(0)->GetMaterial(0);
        mat->SetTexture(tex);
	    mat->EnableAlphaBlend();
        mat->SetSourceBlend(VXBLEND_SRCALPHA);
	    mat->SetDestBlend(VXBLEND_INVSRCALPHA);
	    mat->SetTextureMinMode(VXTEXTUREFILTER_NEAREST);
        mat->SetTextureMagMode(VXTEXTUREFILTER_LINEAR);
        mat->SetTextureAddressMode(VXTEXTURE_ADDRESSWRAP);
        mat->SetTextureBlendMode(VXTEXTUREBLEND_MODULATEALPHA);
        mat->SetDiffuse(col);
		mat->SetEmissive(col/=2);
        context->GetCurrentLevel()->AddObject(body_.body());
        body_.show();
    }

    Title& x(int x){ body_.x(x); return *this; }
    Title& y(int y){ body_.y(y); return *this; }
    Title& z(int z){ body_.z(z); return *this; }
    Title& xyz(int x, int y, int z){ body_.xyz(x, y, z); return *this; }
    Title& parent(CK3dEntity* parent){ body_.parent(parent); return *this; }
    Title& show(){ body_.show(); return *this; }
    Title& hide(){ body_.hide(); return *this; }

private:
    MC body_;
};


int const menu_button_left_padding = 30;
int const menu_button_height_padding = 8;


class Menu{
    typedef std::vector<Button*> Buttons;
public:
    Menu(): enable_(true), shown_(true) {
        Debugger::i().trace("Eat shit");
    }

    void init(int x, int y, int z, char const* name){
        CKDependencies d;
        d.m_Flags = CK_DEPENDENCIES_FULL;
        body_.body() = static_cast<CK3dEntity*>(context->CopyObject(menu_frame, &d, 0, CK_OBJECTCREATION_DYNAMIC));
        body_.xyz(x, y, z);
		Debugger::i().trace(body_.body());
        context->GetCurrentLevel()->AddObject(body_.body());
		title_ = new Title(name);
		title_->parent(body_.body());
		title_->xyz(30, -20, -15);
    }
    Menu& add_button(Button* button){
        buttons_.push_back(&button->parent(body_.body()));
        button->xyz(menu_button_left_padding, -20, -22 -(static_cast<int>(buttons_.size()))*menu_button_height_padding);
        return *this;
    }

    void cycle(CK3dEntity const* picked, bool clicked){

        for(Buttons::iterator i=buttons_.begin(), iend=buttons_.end();
                i!=iend; ++i)
        {
            (**i).leave_focus();
        }

        if(!enable_)
            return;

        Button* focus = find_focus(picked);
        if( focus ){
            focus->on_focus();
            if( clicked ){
                focus->on_click();
            }
        }
    }

    Menu& enable(){ return set_enable(true); }
    Menu& disable(){ return set_enable(false); }
    Menu& set_enable(bool enable = true){ enable_ = enable; return *this; }
	Menu& set_shown(bool shown = true){ shown_ = shown; return *this; }
    bool is_enable() const{ return enable_; }
	bool is_shown() const{ return shown_; }

	Menu& hide(){
		set_shown( false );
		for(Buttons::iterator i=buttons_.begin(), iend=buttons_.end();
            i!=iend; ++i)
        {
            (**i).hide();
        }
		return *this;
	}

	Menu& show(){
        set_shown( true );
        for(Buttons::iterator i=buttons_.begin(), iend=buttons_.end();
            i!=iend; ++i)
        {
            (**i).show();
        }
		return *this;
	}

    Menu& hide_and_disable(){ return hide().disable(); }  //redundant func?
	Menu& show_and_enable(){ return show().enable(); }    //redundant func?

    ~Menu(){
        for(Buttons::iterator i=buttons_.begin(), iend=buttons_.end();
            i!=iend; ++i)
        {
            delete *i;
        }

		delete title_;
    }

private:
    Button* find_focus(CK3dEntity const* picked){
        for(Buttons::iterator i=buttons_.begin(), iend=buttons_.end();
            i!=iend; ++i)
        {
            if( (**i).is_you(picked) )
                return *i;
        }
        return NULL;
    }

private:
	Title* title_;
    Buttons buttons_;
    MC body_;
    bool enable_;
	bool shown_;
};



class GUI{
    typedef std::vector<Menu*> Menus;
public:
	GUI() {
	}
    void cycle(CK3dEntity const* picked, bool clicked){
        for(Menus::iterator i=menus_.begin(), iend=menus_.end();
            i!=iend; ++i)
        {
            (**i).cycle(picked, clicked);
        }
    }

	~GUI() {
		//delete ele of Menus 1 by 1
	}
private:
    Menus menus_;
};

typedef Loki::SingletonHolder<Menu> SM;
typedef Loki::SingletonHolder<GUI> SUI;

class CyclingObject{
public:
    virtual CyclingObject* cycle() = 0;
    virtual ~CyclingObject(){}
};



class CameraWithLight{
public:
	CameraWithLight() {
		light_ = static_cast<CKLight*>(context->CreateObject(CKCID_LIGHT, NULL, CK_OBJECTCREATION_DYNAMIC));
        VxColor c(255, 255, 255);
        light_->SetColor(c);
        light_->SetType(VX_LIGHTPOINT);
        light_->SetLightPower(0.5);
        CKLevel* level = context->GetCurrentLevel();
        level->AddObject(light_);

		camera_ = static_cast<CKCamera*>(context->CreateObject(CKCID_CAMERA, NULL, CK_OBJECTCREATION_DYNAMIC));
		level->AddObject( camera_ );
		context->GetPlayerRenderContext()->AttachViewpointToCamera( camera_ );
	}
    double x() const{ return loc_.x; }
    double y() const{ return loc_.y; }
    double z() const{ return loc_.z; }
    CameraWithLight& x(double x){ loc_.x = x; return adjust_pos(); }
    CameraWithLight& y(double y){ loc_.y = y; return adjust_pos(); }
    CameraWithLight& z(double z){ loc_.z = z; return adjust_pos(); }

	~CameraWithLight() {
		context->DestroyObject( light_, CK_DESTROY_TEMPOBJECT );
		context->DestroyObject( camera_, CK_DESTROY_TEMPOBJECT );
	}

private:
    CameraWithLight& adjust_pos(){
        VxVector vloc(loc_.x, loc_.y, loc_.z);
        camera_->SetPosition(&vloc);
        light_->SetPosition(&vloc);
        return *this;
    }

private:
    CKCamera* camera_;
    CKLight* light_;
    Location loc_;
};



class Scene;
class SceneState{
    friend class Scene;

public:
    virtual void cycle() = 0;

protected:
    SceneState(): scene_(NULL)
    {}
    SceneState(Scene* scene): scene_(scene)
    {}
    void change_owner_state(SceneState*);
    void translate(double);
    virtual ~SceneState(){}

private:
    Scene* scene_;
};

class Nothing: public SceneState{
public:
    virtual void cycle(){}
};

double const speed = 2.0;
namespace direct{
    enum type{
        entering, leaving
    };
}

class Moving: public SceneState{
public:
    Moving(Scene* scene, direct::type direct):
        SceneState(scene), dist_(155), so_far_(0), direct_(direct)  //dist orig: 155
    {}

    virtual void cycle(){
        if(so_far_ < dist_-speed){
            so_far_ += speed;
            if( direct_ == direct::entering )
                SceneState::translate(+speed);
            else
                SceneState::translate(-speed);
        }
        else{
            if( direct_ == direct::entering )
                SceneState::translate(+(dist_-so_far_));
			else {
                SceneState::translate(-(dist_-so_far_));
				SM::Instance().enable();
			}
            SceneState::change_owner_state(new Nothing);
            delete this;
        }
    }

private:
    double dist_, so_far_;
    direct::type direct_;
};



class Scene{
    friend class SceneState;
    typedef std::vector<CyclingObject*> Cyclings;
public:
    Scene(): level_(0), state_(new Nothing) {
		camera_.x( 32.5 ).y( -27.0 ).z( -308.0 );      //orig: 32.5   -27   -308
	}
    void cycle(){
        for(Cyclings::iterator i=cyclings_.begin(); i!=cyclings_.end(); ++i)
        {
            if( (**i).cycle() ){
                delete *i;
                i = --cyclings_.erase(i);
            }
        }
        state_->cycle();
    }
    void add_cycling(CyclingObject* to_be_added){
        cyclings_.push_back(to_be_added);
    }
    void enter(){
        ++level_;
        delete state_;
        state_ = new Moving(this, direct::entering);
    }
    void leave(){
        assert(level_);
        --level_;
        delete state_;
        state_ = new Moving(this, direct::leaving);
    }

	Moving* is_moving(){ return dynamic_cast<Moving*>(state_); }

    ~Scene(){
        for(Cyclings::iterator i=cyclings_.begin(), iend=cyclings_.end();
            i!=iend; ++i)
        {
            delete *i;
        }
        delete state_;
    }

private:
    CameraWithLight camera_;
    Cyclings cyclings_;
    SceneState* state_;
    int level_;
};

typedef Loki::SingletonHolder<Scene> SS;

#endif
