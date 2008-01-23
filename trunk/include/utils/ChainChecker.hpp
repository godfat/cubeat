
typedef multi_array<int, 2> vector_2d;

class ChainChecker{
public:
    static void until_no_chained_cubes(vector_2d<int>& square_colors, int chain_amounts){
        while(not_ok(square_colors, chain_amounts));
    }
private:
    static bool not_ok(vector_2d<int>& square_colors, int chain_amounts){
        std::random_shuffle(square_colors.begin(), square_colors.end());

        for(int y=0, yend=square_colors.height(); y!=yend; ++y)
            for(int x=0, xend=square_colors.width(); x!=xend; ++x)
                if(is_this_good_to_drink(square_colors, x, y, chain_amounts));
                else return true;
        return false;
    }
private:
    friend bool is_this_good_to_drink(vector_2d const&, int, int, int);
    Checker(vector_2d const& square_colors, int x, int y, int chain_amounts):
    square_colors_(square_colors), x_(x), y_(y), chain_amounts_(chain_amounts),
    tested_color_(square_colors_[x_][y_])
    {}

    bool no_left() const{
        if( x_ < chain_amounts_-1 ) return true;
        for(size_type i=1; i<chain_amounts_-1; ++i)
            if( tested_color_ != square_colors_[x_-i][y_]) )
                return true;
        return false;
    }
    bool no_right() const{
        if( x_ > square_colors_.width() - chain_amounts_ ) return true;
        for(size_type i=1; i<chain_amounts_-1; ++i)
            if( tested_color_ != square_colors_[x_+i][y_]) )
                return true;
        return false;
    }
    bool no_up() const{
        if( y_ > square_colors_.height() - chain_amounts_ ) return true;
        for(size_type i=1; i<chain_amounts_-1; ++i)
            if( tested_color_ != square_colors_.[x_][y_+i]) )
                return true;
        return false;
    }
    bool no_down() const{
        if( y_ < chain_amounts_-1 ) return true;
        for(size_type i=1; i<chain_amounts_-1; ++i)
            if( tested_color_ != square_colors_.[x_][y_-i]) )
                return true;
        return false;
    }

private:
    vector_2d const& square_colors_;
    typedef vector_2d::size_type size_type;
    size_type const x_, y_;
    size_type const chain_amounts_;
    int const tested_color_;
};

bool is_this_good_to_drink(vector_2d const& square_colors, int x, int y, int chain_amounts){
    ChainChecker c(square_colors, x, y, chain_amounts);
    return c.no_left() && c.no_right() && c.no_up() && c.no_down();
}

