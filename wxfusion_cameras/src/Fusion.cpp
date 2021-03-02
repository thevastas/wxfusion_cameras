#include "Fusion.h"
void Fusion::init(cv::Mat nir_img, cv::Mat lwir_img, int offsetx, int offsety, double ratio, bool palette) {

    if (offsetx >= 0) {
        if (offsety >= 0) { //both positive
            //X
            m_nirx1 = offsetx;
            m_nirx2 = nir_img.cols;
            m_lwirx1 = 0;
            m_lwirx2 = lwir_img.cols - offsetx;
            //Y
            m_niry1 = offsety;
            m_niry2 = nir_img.rows;
            m_lwiry1 = 0;
            m_lwiry2 = lwir_img.rows - offsety;
        }
        else { // x positive, y negative
            //X
            m_nirx1 = abs(offsetx);
            m_nirx2 = nir_img.cols;
            m_lwirx1 = 0;
            m_lwirx2 = lwir_img.cols - abs(offsetx);
            //Y
            m_niry1 = 0;
            m_niry2 = nir_img.rows - abs(offsety);
            m_lwiry1 = abs(offsety);
            m_lwiry2 = lwir_img.rows;
        }
    }
    else
        if (offsety >= 0) { //x negative, y positive
            //X
            m_nirx1 = 0;
            m_nirx2 = nir_img.cols - abs(offsetx);
            m_lwirx1 = abs(offsetx);
            m_lwirx2 = lwir_img.cols;
            //Y
            m_niry1 = abs(offsety);
            m_niry2 = nir_img.rows;
            m_lwiry1 = 0;
            m_lwiry2 = lwir_img.rows - offsety;
        }
        else { // both negative
            //X
            m_nirx1 = 0;
            m_nirx2 = nir_img.cols - abs(offsetx);
            m_lwirx1 = abs(offsetx);
            m_lwirx2 = lwir_img.cols;
            //Y
            m_niry1 = 0;
            m_niry2 = nir_img.rows - abs(offsety);
            m_lwiry1 = abs(offsety);
            m_lwiry2 = lwir_img.rows;
        }
    m_fused_columns = m_nirx2 - m_nirx1;
    m_fused_rows = m_niry2 - m_niry1;


    //if (0 > nirx1) printf("nirx1 (%i) < 0\n", nirx1);
    //if (0 > niry1) printf("niry1 (%i) < 0\n", niry1);
    //if (0 > lwirx1) printf("lwirx1 (%i) < 0\n", lwirx1);
    //if (0 > lwiry1) printf("lwiry1 (%i) < 0\n", lwiry1);
    //if (0 > fused_columns) printf("nir lwir width (%i) < 0\n", fused_columns);
    //if (0 > fused_rows) printf("nir lwir width (%i) < 0\n", fused_rows);
    //if ((nirx1 + fused_columns) > nir_img.cols) printf("nirx1 (%i) + fused_columns (%i) > nir_img.cols (%i)\n", nirx1, fused_columns, nir_img.cols);
    //if ((niry1 + fused_rows) > nir_img.rows) printf("niry1 (%i) + fused_rows (%i) > nir_img.rows (%i)\n", niry1, fused_rows, nir_img.rows);
    //if ((lwirx1 + fused_columns) > lwir_img.cols) printf("lwirx1  (%i)+ fused_columns (%i) > lwir_img.cols (%i)\n", lwirx1, fused_columns, lwir_img.cols);
    //if ((lwiry1 + fused_rows) > lwir_img.rows) printf("lwiry1 (%i) + fused_rows (%i) > lwir_img.rows (%i)\n", lwiry1, fused_rows, lwir_img.rows);

    
    return;
    //return fused_img;

}
cv::Mat Fusion::fuse_offset(cv::Mat nir_img, cv::Mat lwir_img) {
    m_nir_roi = nir_img(cv::Rect(m_nirx1, m_niry1, m_fused_columns, m_fused_rows));
    m_lwir_roi = lwir_img(cv::Rect(m_lwirx1, m_lwiry1, m_fused_columns, m_fused_rows));

    m_nir_poi = m_nir_roi.clone();
    m_lwir_poi = m_lwir_roi.clone();

    if (m_palette) {

        const cv::Mat zero_img = cv::Mat::zeros(m_nir_poi.rows, m_nir_poi.cols, CV_8UC1);
        std::vector<cv::Mat> images(3);
        images.at(0) = zero_img;
        images.at(1) = m_nir_poi;
        images.at(2) = m_lwir_poi;
        cv::merge(images, m_fused_img);
        //applyColorMap(lwir_poi, lwir_poi, cv::COLORMAP_HOT);
        //cvtColor(nir_poi, nir_poi, cv::COLOR_GRAY2BGR);
        //applyColorMap(nir_poi, nir_poi, cv::COLORMAP_BONE);
    }
    else {
        m_nir_poi = m_nir_poi * m_ratio;
        m_lwir_poi = m_lwir_poi * (1 - m_ratio);
        m_fused_img = m_nir_poi + m_lwir_poi;
    }
    //return;
    return m_fused_img;
}

