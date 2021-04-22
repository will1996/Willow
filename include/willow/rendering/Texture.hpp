//
// Created by Will Chambers on 4/15/21.
//

#ifndef WILLOW_TEXTURE_HPP
#define WILLOW_TEXTURE_HPP
#include"willow/root/Root.hpp"
#include"willow/data/View.hpp"
namespace wlo{
   class Texture:public Tag{
   public:
       Texture() = default;
        Texture(std::string loadFile);
        Texture(data::Type t, size_t x, size_t y = 1, size_t z = 1);
       void* data();
       View view() const;
       size_t size()const ;
       size_t width()const ;
       size_t height()const ;
       size_t depth()const ;

       data::Type texelFormat()const;
   private:
      struct AbstractStorage{
          virtual void free() = 0;
          virtual void* data() = 0;
          virtual View view() = 0;
          virtual void fill(void *) = 0;
          virtual size_t size() = 0;
          virtual UniquePointer<AbstractStorage> copy() = 0;
      };

      template<typename T>
      struct Storage : public AbstractStorage{
        Storage(size_t x, size_t y, size_t z):texels(x*y*z){ }
        void free()override{
            texels.clear();
        }
        void* data() override{
               return texels.data();
                }

        View view() override{
              return View(texels);
          }
        size_t size() override{
            return texels.size();
        }
        void fill(void*src) override{
            memcpy(texels.data(),static_cast<T*>(src),texels.size()*sizeof(T));
        }

          UniquePointer<AbstractStorage> copy() override{
            return CreateUniquePointer<Storage<T>>(*this);
        };
        std::vector<T> texels;
      };
       data::Type format;
       size_t m_width;
       size_t m_height;
       size_t m_depth;
       UniquePointer<AbstractStorage>  m_storage;
   };


};
#endif //WILLOW_TEXTURE_HPP
