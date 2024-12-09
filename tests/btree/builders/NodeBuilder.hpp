#ifndef MKVDB_TESTS_BTREE_BUILDERS_NODE_BUILDER_HPP_
#define MKVDB_TESTS_BTREE_BUILDERS_NODE_BUILDER_HPP_

#include "mkvdb/common/Types.hpp"

#include "mkvdb/pager/Page.hpp"

#include "../../utils/Random.hpp"
#include "../../utils/RandomBlob.hpp"
#include "mkvdb/btree/BTree.hpp"
#include "mkvdb/btree/Node.hpp"

#include <cstdint>
#include <limits>
#include <optional>

namespace mkvdb::tests::btree::builders
{

    class NodeBuilder
    {
    public:
        // Constructor
        NodeBuilder()
        : items_count_(0)
        {
            key_size_   = utils::ChooseRandom({ 2, 4, 8 });
            value_size_ = utils::ChooseRandom({ 2, 4, 8, 16, 32 });
            page_size_ =
              utils::ChooseRandom({ 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536 });
            page_index_ = utils::ChooseRandom(0, 1) == 0
                            ? 0
                            : utils::ChooseRandom<pager::Page::PageIndex>(
                                0,
                                std::numeric_limits<pager::Page::PageIndex>::max());
        }

        NodeBuilder& WithItemsCount(std::uint32_t items_count)
        {
            items_count_ = items_count;
            return *this;
        }

        NodeBuilder& WithKeySize(mkvdb::common::KeyValueSize key_size)
        {
            key_size_ = key_size;
            return *this;
        }

        NodeBuilder& WithValueSize(mkvdb::common::KeyValueSize value_size)
        {
            value_size_ = value_size;
            return *this;
        }

        NodeBuilder& WithPageSize(mkvdb::pager::Page::PageSize page_size)
        {
            page_size_ = page_size;
            return *this;
        }

        NodeBuilder& WithPageIndex(mkvdb::pager::Page::PageIndex page_index)
        {
            page_index_ = page_index;
            return *this;
        }

        mkvdb::btree::Node& Build()
        {
            if(!node_)
            {
                btree_.emplace(key_size_, value_size_);
                page_ = std::make_shared<mkvdb::pager::Page>(page_index_, page_size_);
                node_.emplace(*btree_, page_, value_size_, true);

                for(std::uint32_t x = 0; x < items_count_; ++x)
                {
                    utils::RandomBlob key_blob(key_size_);
                    utils::RandomBlob value_blob(value_size_);
                    node_->Insert(key_blob.data(), value_blob.data());
                }
            }
            return *node_;
        }

    private:
        mkvdb::common::KeyValueSize key_size_;
        mkvdb::common::KeyValueSize value_size_;
        mkvdb::pager::Page::PageIndex page_index_;
        mkvdb::pager::Page::PageSize page_size_;
        std::uint32_t items_count_;

        std::optional<mkvdb::btree::BTree> btree_;
        std::shared_ptr<mkvdb::pager::Page> page_;
        std::optional<mkvdb::btree::Node> node_;
    };

} // namespace mkvdb::tests::btree::builders

#endif // MKVDB_TESTS_BTREE_BUILDERS_NODE_BUILDER_HPP_