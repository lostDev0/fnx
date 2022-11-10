namespace fnx
{
bool button::do_mouse_release( FNX_BUTTON button, double x, double y )
{
    auto ret = widget::do_mouse_release( button, x, y );
    if ( ret )
    {
        // someone absorbed it
        auto icon = _children[1];
        if ( icon )
        {
            icon->toggle();
        }
    }
    return false;
}
}