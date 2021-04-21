//
// Created by W on 10/7/20.
//

#ifndef WILLOW_CONSOLECORE_HPP
#define WILLOW_CONSOLECORE_HPP
#include"willow/root/Root.hpp"
#include"willow/rendering/RenderDataTypes.hpp"
#include"willow/input/KeyCodes.hpp"
#include"Font.hpp"
namespace wlo {
    class ConsoleCore {
    public:
        ConsoleCore();

        enum CursorMovement {
            UP, LEFT, DOWN, RIGHT
        };

        const std::vector<wlo::Vertex3D> getVertexBuffer();

        const std::vector<uint32_t> getIndexBuffer();

        std::pair<size_t, size_t> getCursorPos();

        size_t numColsInRow();

        size_t NumRows();

        void moveCursor(CursorMovement);

        void placeChar(wlo::Key::Code code,wlo::Key::Modifier);

        void nextLine();

        void deleteChar();

        std::string flushInputBuffer();

    private:
        const static size_t fontWidth = 20;
        const static size_t fontHeight = 50;
        struct CursorPos {
            size_t row;
            size_t col;
        };

        std::vector<char> m_inputBuffer;
        wlo::Font fontMap;
        std::vector<std::vector<wlo::Font::Glyph> > m_characterBuffer;

        void insertChar(CursorPos, Font::Glyph);//inserts a character UNDERNEATH the cursor
        void removeChar(CursorPos);// deletes a character UNDERNEATH the cursor

        CursorPos m_cursorPos;


        std::vector<wlo::Vertex3D> buildVertexBuffer();

        std::vector<uint32_t> buildIndexBuffer();

        std::vector<wlo::Vertex3D> offsetVertsToCursorPos(const std::vector<wlo::Vertex3D> &verts);

        std::vector<wlo::Vertex3D> offsetVerts(const std::vector<wlo::Vertex3D> &verts, size_t row, size_t col);

        std::vector<wlo::Vertex3D> colorVerts(const std::vector<wlo::Vertex3D> &verts, glm::vec3 color);


        struct CursorModel {
            const std::vector<wlo::ColorVertex2D> verts{
            };
            const std::vector<uint32_t> inds{0, 1, 2, 2, 3, 0};
        };
        CursorModel m_cursorModel;

        struct EmptyModel {
            const std::vector<wlo::ColorVertex3D> verts{
            };
            const std::vector<uint32_t> inds{0, 1, 2, 2, 3, 0};
        };

        EmptyModel emptyModel;
    };

}
#endif //WILLOW_CONSOLECORE_HPP
