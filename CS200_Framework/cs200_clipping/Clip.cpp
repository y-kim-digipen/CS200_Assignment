/** Name      : Yoonki Kim
// Assignment : Clipping Programming Assignment
// Course     : CS200
// Term & Year: Fall 2020
**/
#include "Clip.h"

bool Clip::operator()(std::vector<Hcoord>& vertices)
{
    bool has_vertex_in_clip = false;

    for (const auto& half_space : half_spaces)
    {
        const int vertices_size = static_cast<int>(vertices.size());

        //to make lines, need at least two vertices
        if(vertices_size <= 1)
        {
            return false;
        }

        std::vector<Hcoord> clipped;
        clipped.reserve(2 * vertices_size);
        float last_eval = dot(half_space, vertices[0]);

        if(last_eval <= 0.f)
        {
            clipped.push_back(vertices[0]);
            has_vertex_in_clip |= true;
        }

        for (int i = 1; i <= vertices_size; ++i)
        {
            const Hcoord cur_vertex = i < vertices_size ? vertices[i] : vertices[0];
            const float cur_eval = dot(half_space, cur_vertex);

            if(last_eval <= 0)
            {
                //in - out
                if(cur_eval > 0)
                {
                    const float t = last_eval / (last_eval - cur_eval);
                    const Hcoord intersection = (1 - t) * vertices[i - 1] + t * cur_vertex;
                    clipped.push_back(intersection);
                }
                //in - in
                else
                {
                    if(i != vertices_size)
                    {
                        clipped.push_back(cur_vertex);
                    }
                }
                has_vertex_in_clip |= true;
            }
            else
            {
                //out - in
                if (cur_eval <= 0)
                {
                    const float t = last_eval / (last_eval - cur_eval);
                    const Hcoord intersection = (1 - t) * vertices[i - 1] + t * cur_vertex;
                    clipped.push_back(intersection);
                    clipped.push_back(cur_vertex);
                    has_vertex_in_clip |= true;
                }
                //out - out -- does not have to do anything
            }
            last_eval = cur_eval;
        }
        clipped.shrink_to_fit();
        vertices = clipped;
    }

    return has_vertex_in_clip;
}
