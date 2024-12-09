#ifndef MKVDB_PAGER_BUFFER_WITH_HEADER_HPP_
#define MKVDB_PAGER_BUFFER_WITH_HEADER_HPP_

namespace mkvdb::pager
{
    // TODO : Delete this class if it is not used.
    template<typename Header>
    class BufferWithHeader
    {
    public:
    private:
        Header header_;
    };
} // namespace mkvdb::pager

#endif // MKVDB_PAGER_BUFFER_WITH_HEADER_HPP_