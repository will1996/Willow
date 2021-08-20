//
// Created by W on 10/7/20.
//

#include "willow/console/ConsoleCore.hpp"
#include "glm/gtc/matrix_transform.hpp"
namespace wlo {
    ConsoleCore::ConsoleCore():fontMap("hello") {
        m_characterBuffer.resize(1);
    }

    const std::vector<wlo::Vec3> ConsoleCore::getVertexBuffer() {
        return buildVertexBuffer();
    }

    const std::vector<uint32_t> ConsoleCore::getIndexBuffer() {
        return buildIndexBuffer();
    }

    void ConsoleCore::moveCursor(ConsoleCore::CursorMovement mov) {
        std::cout << "position before move: " << std::endl;
        std::cout << "cursorPos row: " << m_cursorPos.row << std::endl;
        std::cout << "cursorPos col: " << m_cursorPos.col << std::endl;
        switch (mov) {
            case UP:
                std::cout << "pressed up " << std::endl;
                if (m_cursorPos.row > 0)
                    m_cursorPos.row -= 1;
                break;
            case DOWN:
                std::cout << "pressed down " << std::endl;
                if (m_cursorPos.row < m_characterBuffer.size() - 1)
                    m_cursorPos.row++;
                break;
            case LEFT:
                std::cout << "pressed left " << std::endl;
                if (m_cursorPos.col > 0)
                    m_cursorPos.col -= 1;
                break;
            case RIGHT:
                std::cout << "pressed right " << std::endl;
                if (m_cursorPos.col < m_characterBuffer[m_cursorPos.row].size())
                    m_cursorPos.col++;
                break;
        }
        std::cout << "position after move: " << std::endl;
        std::cout << "cursorPos row: " << m_cursorPos.row << std::endl;
        std::cout << "cursorPos col: " << m_cursorPos.col << std::endl;

    }


    void ConsoleCore::placeChar(wlo::Key::Code code,wlo::Key::Modifier mod) {
        std::cout << "placing char" << std::endl;
        std::vector<wlo::Vec3> verts;
//        insertChar(m_cursorPos, FontMap::Character{wlo::Key::toText(code,mod).c_str()[0], colorVerts(m_cursorModel.verts, {1, 0, 0}), m_cursorModel.inds});
        moveCursor(RIGHT);
    }

    void ConsoleCore::deleteChar() {
        moveCursor(LEFT);
        removeChar(m_cursorPos);
    }

    std::string ConsoleCore::flushInputBuffer() {
        std::stringstream ss;
        for (auto &c : m_characterBuffer[m_cursorPos.row])
            ss<<c.textValue;
        return ss.str();
    }


    std::vector<wlo::Vec3> ConsoleCore::buildVertexBuffer() {
        std::vector<wlo::Vec3> vertexBuffer;
//        std::vector<wlo::Vec3> cursorVerts = offsetVertsToCursorPos(m_cursorModel.verts);
 //       vertexBuffer.insert(vertexBuffer.begin(), cursorVerts.begin(), cursorVerts.end());
        for (size_t row = 0; row < m_characterBuffer.size(); row++)
            for (size_t col = 0; col < m_characterBuffer[row].size(); col++) {
                //std::vector<wlo::Vec3> offsetCharVerts = offsetVerts(m_characterBuffer[row][col].verts, row, col);
              //  vertexBuffer.insert(vertexBuffer.end(), offsetCharVerts.begin(), offsetCharVerts.end());
            }

        return vertexBuffer;
    }

    std::vector<uint32_t> ConsoleCore::buildIndexBuffer() {
        std::vector<uint32_t> indexBuffer;
        size_t offset = 0;
        for (auto &row : m_characterBuffer)
            for (auto &character: row) {
                for (auto &ind : character.inds)
                    indexBuffer.push_back(ind + offset);
                offset += character.verts.size();
            }
        for (auto &ind : m_cursorModel.inds)
            indexBuffer.push_back(ind + offset);
        return indexBuffer;
    }

    std::vector<wlo::Vec3> ConsoleCore::offsetVertsToCursorPos(const std::vector<wlo::Vec3> &verts) {
        return offsetVerts(verts, m_cursorPos.row, m_cursorPos.col);
    }

    std::vector<wlo::Vec3>
    ConsoleCore::offsetVerts(const std::vector<wlo::Vec3> &verts, size_t row, size_t col) {
        std::vector<wlo::Vec3> outVerts;
        outVerts.reserve(verts.size());
  //      for (auto vert : verts)
 //           outVerts.push_back(glm::translate(glm::mat4x4(1), glm::vec3(fontWidth * col, fontHeight * row, 0)) * vert);
        return outVerts;
    }


    void ConsoleCore::removeChar(ConsoleCore::CursorPos pos) {
        if (!(pos.col < numColsInRow()))
            return;
        m_characterBuffer[pos.row].erase(m_characterBuffer[pos.row].begin() + pos.col);

    }

    std::vector<wlo::Vec3> ConsoleCore::colorVerts(const std::vector<wlo::Vec3> &verts, glm::vec3 color) {
        std::vector<wlo::Vec3> outVerts;
        outVerts.reserve(verts.size());
        for (const auto &vert : verts) {
//            outVerts.push_back(wlo::Vec3::setColor(vert, color));
        }

        return outVerts;

    }

    size_t ConsoleCore::numColsInRow() {
        return m_characterBuffer[m_cursorPos.row].size();
    }

    std::pair<size_t, size_t> ConsoleCore::getCursorPos() {
        return std::pair<size_t, size_t>{m_cursorPos.row, m_cursorPos.col};
    }

    size_t ConsoleCore::NumRows() {
        return m_characterBuffer.size();
    }

    void ConsoleCore::nextLine() {
        m_characterBuffer.resize(m_characterBuffer.size() + 1);
        m_cursorPos.row++;
        m_cursorPos.col = 0;
    }

}