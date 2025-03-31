namespace yap
{
    enum class Axis
    {
        X,
        Y
    };

    Axis GetAxisComplement(Axis axis)
    {
        return (axis == Axis::X) ? Axis::Y : Axis::X;
    }
}