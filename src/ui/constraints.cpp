namespace fnx
{
class center_label_vert_constraint : public relative_vert_constraint
{
public:
    center_label_vert_constraint() : relative_vert_constraint( .5f ) {}
    virtual ~center_label_vert_constraint() {};
    center_label_vert_constraint( const center_label_vert_constraint& other ) = default;
    center_label_vert_constraint( center_label_vert_constraint&& other ) = default;
    center_label_vert_constraint& operator=( const center_label_vert_constraint& other ) = default;
    center_label_vert_constraint& operator=( center_label_vert_constraint&& other ) = default;

    virtual std::unique_ptr<constraint> clone() override
    {
        return std::make_unique<center_label_vert_constraint>( *this );
    };

    virtual vector4 apply( const vector4& parent, const vector4& child ) override
    {
        auto n_child = child;
        auto child_height = child.w;
        auto parent_height = parent.w;
        n_child.y = ( parent_height / 2.f ) - ( child_height );
        return n_child;
    }
};

class center_label_horz_constraint : public relative_horz_constraint
{
public:
    center_label_horz_constraint() : relative_horz_constraint( .5f ) {}
    virtual ~center_label_horz_constraint() {};
    center_label_horz_constraint( const center_label_horz_constraint& other ) = default;
    center_label_horz_constraint( center_label_horz_constraint&& other ) = default;
    center_label_horz_constraint& operator=( const center_label_horz_constraint& other ) = default;
    center_label_horz_constraint& operator=( center_label_horz_constraint&& other ) = default;

    virtual std::unique_ptr<constraint> clone() override
    {
        return std::make_unique<center_label_horz_constraint>( *this );
    };

    virtual vector4 apply( const vector4& parent, const vector4& child ) override
    {
        auto n_child = child;
        auto child_width = child.z;
        auto parent_width = parent.z;
        n_child.x = ( parent_width / 2.f ) - ( child_width );
        return n_child;
    }
};

class top_label_vert_constraint : public relative_vert_constraint
{
public:
    top_label_vert_constraint() : relative_vert_constraint( .5f ) {}
    virtual ~top_label_vert_constraint() {};
    top_label_vert_constraint( const top_label_vert_constraint& other ) = default;
    top_label_vert_constraint( top_label_vert_constraint&& other ) = default;
    top_label_vert_constraint& operator=( const top_label_vert_constraint& other ) = default;
    top_label_vert_constraint& operator=( top_label_vert_constraint&& other ) = default;

    virtual std::unique_ptr<constraint> clone() override
    {
        return std::make_unique<top_label_vert_constraint>( *this );
    };

    virtual vector4 apply( const vector4& parent, const vector4& child ) override
    {
        auto n_child = child;
        auto child_height = child.w;
        auto parent_height = parent.w;
        n_child.y = ( parent_height ) - ( child_height );
        return n_child;
    }
};

// TODO : fix the other constraints for labels

void apply_label_constraints( fnx::widget& widget, alignment align )
{
    constraints c;
    switch ( align )
    {
        case alignment::right:
            c.add( relative_horz_constraint( 1.f ) );
            c.add( center_label_vert_constraint{} );
            break;
        case alignment::top:
            c.add( relative_vert_constraint( 1.f ) );
            c.add( center_label_horz_constraint() );
            break;
        case alignment::bottom:
            c.add( relative_vert_constraint( 0.f ) );
            c.add( center_label_horz_constraint() );
            break;
        case alignment::bottom_left:
            c.add( relative_vert_constraint( 0.f ) );
            c.add( relative_horz_constraint( 0.f ) );
            break;
        case alignment::bottom_right:
            c.add( relative_vert_constraint( 0.f ) );
            c.add( relative_horz_constraint( 1.f ) );
            break;
        case alignment::top_left:
            c.add( top_label_vert_constraint() );
            c.add( relative_horz_constraint( 0.f ) );
            break;
        case alignment::top_right:
            c.add( relative_vert_constraint( 1.f ) );
            c.add( relative_horz_constraint( 1.f ) );
            break;
        case alignment::center:
            c.add( center_label_horz_constraint() );
            c.add( center_label_vert_constraint{} );
            break;
        case alignment::left:
            c.add( relative_horz_constraint( 0.f ) );
            c.add( center_label_vert_constraint{} );
            break;
    }
    widget.set_constraints( c );
}

vector4 vert_aspect_constraint::apply( const vector4& parent, const vector4& child )
{
    auto n_child = child;
    auto [win, _] = singleton<window>::acquire();
    n_child.w = ( child.z * win.get_aspect_ratio() * _modifier );
    return n_child;
}

vector4 horz_aspect_constraint::apply( const vector4& parent, const vector4& child )
{
    auto n_child = child;
    auto [win, _] = singleton<window>::acquire();
    auto ratio = win.get_aspect_ratio();
    // set width based on the aspect ratio
    // width/height = ratio
    auto height = child.w;
    n_child.z = ( height / ratio * _modifier );
    return n_child;
}
}
