#pragma once

namespace fnx
{
/// @brief A user interface widget to display a string on the screen.
class label : public fnx::block
{
public:
    label( const std::string& font, const std::string& text, const std::string& name = create_type_name<fnx::label>() )
        : block( fnx::colors::transparent, name )
        , _dirty_cache { true }
        , _label_color( fnx::colors::white )
        , _label_font()
        , _label()
        , _label_font_name( font )
        , _text_alignment( alignment::left )
        , _label_size{ 0.f, 0.f }
    {
        set_text( text );
        disable_events();	// block will absorb
    }
    label( fnx::font_handle font, const std::string& text, const std::string& name = create_type_name<fnx::label>() )
        : block( fnx::colors::transparent, name )
        , _dirty_cache{ true }
        , _label_color( fnx::colors::white )
        , _label_font( font )
        , _label_font_name( font->get_name() )
    {
        disable_events();	// block will absorb
    }
    virtual ~label() = default;

    /// @brief Command the label to recalculate its size.
    virtual void on_parent_change( reactphysics3d::decimal parent_x, reactphysics3d::decimal parent_y,
                                   reactphysics3d::decimal parent_width, reactphysics3d::decimal parent_height ) override;

    /// @brief Set the displayed label string.
    virtual void set_text( const std::string& label );

    /// @brief Override the constraint system for the font height.
    virtual void set_text_size( reactphysics3d::decimal height );

    /// @breif Override the constraint system for the font height using pixels.
    /// @brief This value will be converted to a ratio for window resize.
    virtual void set_text_size_in_pixels( size_t px );

    /// @breif Override the constraint system for the font height using point value (72 points per inch).
    /// @brief This value will be converted to a ratio for window resize.
    virtual void set_text_size_in_points( size_t pt );

    /// @brief Return the label's string.
    const auto& get_label()
    {
        return _label;
    }

    /// @brief Set the foreground color of the label.
    void set_text_color( const fnx::colors::rgba& color );

    /// @brief Set the foreground color of the label.
    void set_border_color( const fnx::colors::rgba& color );

    /// @brief Set the text alignment.
    void set_text_alignment( alignment align )
    {
        _text_alignment = align;
    }

    /// @brief Add bold styling to the label.
    void set_bold()
    {
        _bold = true;
    }

    /// @brief Draw the label.
    void render( camera_handle camera, matrix4x4 parent_matrix ) override;

    void set_border_width( reactphysics3d::decimal width );
    void set_border_edge( reactphysics3d::decimal edge );
    void set_border_offset( reactphysics3d::decimal x, reactphysics3d::decimal y );

protected:
    bool _dirty_cache{ true };				/// label needs to recalculate its model
    bool _bold{ false };					/// is bold
    fnx::colors::rgba _label_color;			/// foreground color
    fnx::colors::rgba _border_color;		/// border color
    reactphysics3d::decimal _border_width{ 0.f };
    reactphysics3d::decimal _border_edge{ 0.1f };				/// border edge transition value (this MUST be above 0)
    fnx::vector2 _border_offset{ 0.f, 0.f};
    font_handle _label_font;				/// font to calculate the model
    std::string _label;						/// current label
    alignment _text_alignment{alignment::left};	/// alignment of the font
    std::string _label_font_name;			/// font family name
    reactphysics3d::decimal _font_size{ 0.006f };				/// font size only used if set
    fnx::vector2 _label_size{ 0.f, 0.f };			/// dimensions of the label model
    std::vector<std::pair<reactphysics3d::decimal, std::string>>
            _lines;	/// each line of the _label broken to fit (width,string)

    /// @brief Recalculate the model of the label.
    void update_label_model();
};

using label_handle = fnx::widget_handle_t<fnx::label>;
}
