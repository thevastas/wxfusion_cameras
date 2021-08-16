#include "Fusion.h"
void Fusion::init(cv::UMat nir_img, cv::UMat lwir_img, int offsetx, int offsety, double ratio, bool palette) {
    m_palette = palette;
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
cv::UMat Fusion::fuse_offset(cv::UMat nir_img, cv::UMat lwir_img, int weight) {
    m_nir_roi = nir_img(cv::Rect(m_nirx1, m_niry1, m_fused_columns, m_fused_rows));
    m_lwir_roi = lwir_img(cv::Rect(m_lwirx1, m_lwiry1, m_fused_columns, m_fused_rows));

    m_nir_poi = m_nir_roi.clone();
    m_lwir_poi = m_lwir_roi.clone();

    cv::cvtColor(m_nir_poi, m_nir_poi, cv::COLOR_BGR2GRAY);
    cv::cvtColor(m_lwir_poi, m_lwir_poi, cv::COLOR_BGR2GRAY);
    //cv::cvtColor(nir_img, nir_img, cv::COLOR_BGR2GRAY);
    //cv::cvtColor(lwir_img, lwir_img, cv::COLOR_BGR2GRAY);
    //warning
    m_palette = true;
    double weight_nir = weight / 100.0;
    double weight_lwir = (100 - weight) / 100.0;
    if (m_palette) {

        const cv::UMat zero_img = cv::UMat::zeros(m_nir_poi.rows, m_nir_poi.cols, CV_8UC1);
        std::vector<cv::UMat> images(3);
        
        images.at(0) = zero_img;
        cv::multiply(m_nir_poi, weight_nir, scaled_nir, 1);
        images.at(1) = scaled_nir;
        //images.at(1) = nir_img;
       //images.at(1) = zero_img;
        cv::multiply(m_lwir_poi, weight_lwir, scaled_lwir, 1);
        images.at(2) = scaled_lwir;
        //images.at(2) = lwir_img;
        cv::merge(images, m_fused_img);
        m_fused_img.convertTo(m_fused_img, CV_8UC3);
        //applyColorMap(lwir_poi, lwir_poi, cv::COLORMAP_HOT);
        //cvtColor(nir_poi, nir_poi, cv::COLOR_GRAY2BGR);
        //applyColorMap(nir_poi, nir_poi, cv::COLORMAP_BONE);
    }
    else {
        m_fused_img = lwir_img.clone();
        //m_nir_poi = m_nir_poi * m_ratio;
        //m_lwir_poi = m_lwir_poi * (1 - m_ratio);
        //m_fused_img = m_nir_poi + m_lwir_poi;
    }
    //return;
    return m_fused_img;
}

