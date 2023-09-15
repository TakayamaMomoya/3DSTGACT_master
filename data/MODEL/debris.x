xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 44;
 8.49101;8.82947;0.17446;,
 8.47763;8.82947;-1.35836;,
 -8.48655;8.82947;-1.21031;,
 -8.47317;8.82947;0.32251;,
 -8.46670;-8.80223;1.06392;,
 -8.48008;-8.80223;-0.46890;,
 8.48411;-8.80223;-0.61694;,
 8.49749;-8.80223;0.91588;,
 -8.46013;3.53574;1.81776;,
 -8.47317;8.82947;0.32251;,
 -8.48655;8.82947;-1.21031;,
 -8.47332;3.51164;0.30624;,
 -8.47717;-0.79024;-0.13580;,
 -8.46379;-0.81877;1.39702;,
 -8.47332;3.51164;0.30624;,
 -8.48655;8.82947;-1.21031;,
 8.48856;3.81033;-0.10656;,
 8.48702;-1.18039;-0.28385;,
 -8.47717;-0.79024;-0.13580;,
 8.48856;3.81033;-0.10656;,
 8.47763;8.82947;-1.35836;,
 8.49101;8.82947;0.17446;,
 8.50177;3.83446;1.40653;,
 8.50039;-1.20891;1.24897;,
 8.48702;-1.18039;-0.28385;,
 8.50177;3.83446;1.40653;,
 -8.47317;8.82947;0.32251;,
 -8.46013;3.53574;1.81776;,
 -8.46379;-0.81877;1.39702;,
 8.50039;-1.20891;1.24897;,
 -8.48763;-4.71936;-1.33403;,
 -8.48008;-8.80223;-0.46890;,
 -8.46670;-8.80223;1.06392;,
 -8.47415;-4.78198;0.20989;,
 8.47684;-5.01978;-1.44986;,
 -8.48008;-8.80223;-0.46890;,
 -8.48763;-4.71936;-1.33403;,
 8.49032;-5.08243;0.09476;,
 8.49749;-8.80223;0.91588;,
 8.48411;-8.80223;-0.61694;,
 8.47684;-5.01978;-1.44986;,
 -8.47415;-4.78198;0.20989;,
 -8.46670;-8.80223;1.06392;,
 8.49032;-5.08243;0.09476;;
 
 18;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;8,11,12,13;,
 4;14,15,1,16;,
 4;14,16,17,18;,
 4;19,20,21,22;,
 4;19,22,23,24;,
 4;25,0,26,27;,
 4;25,27,28,29;,
 4;30,31,32,33;,
 4;30,33,13,12;,
 4;34,6,35,36;,
 4;34,36,18,17;,
 4;37,38,39,40;,
 4;37,40,24,23;,
 4;41,42,7,43;,
 4;41,43,29,28;;
 
 MeshMaterialList {
  1;
  18;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.360000;0.360000;0.360000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  22;
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  -0.999962;-0.000000;0.008726;,
  -0.009299;0.181187;-0.983405;,
  0.999962;0.000000;-0.008726;,
  -0.999962;-0.000000;0.008726;,
  -0.015452;-0.095937;-0.995267;,
  0.999962;0.000000;-0.008727;,
  -0.999962;-0.000000;0.008726;,
  -0.005658;0.041311;-0.999130;,
  0.999962;0.000000;-0.008727;,
  -0.999962;0.000000;0.008726;,
  -0.013521;-0.258161;-0.966007;,
  0.999962;-0.000000;-0.008728;,
  0.015414;0.097241;0.995142;,
  0.013508;0.255694;0.966663;,
  0.009322;-0.176128;0.984323;,
  -0.999962;0.000000;0.008727;,
  -0.009436;-0.211180;-0.977402;,
  0.999962;0.000000;-0.008727;,
  0.005691;-0.038501;0.999242;,
  0.009420;0.211684;0.977293;;
  18;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;5,11,11,5;,
  4;5,5,2,2;,
  4;6,12,12,6;,
  4;6,6,3,3;,
  4;7,13,13,7;,
  4;7,7,4,4;,
  4;14,15,15,14;,
  4;14,14,16,16;,
  4;8,17,17,8;,
  4;8,8,2,2;,
  4;9,18,18,9;,
  4;9,9,3,3;,
  4;10,19,19,10;,
  4;10,10,4,4;,
  4;20,21,21,20;,
  4;20,20,16,16;;
 }
 MeshTextureCoords {
  44;
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.208563;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.209321;,
  1.000000;0.378653;,
  0.000000;0.380122;,
  0.000000;0.209321;,
  0.000000;0.000000;,
  1.000000;0.199943;,
  1.000000;0.398753;,
  0.000000;0.378653;,
  0.000000;0.199943;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.199147;,
  1.000000;0.400222;,
  0.000000;0.398753;,
  0.000000;0.199147;,
  1.000000;0.000000;,
  1.000000;0.208563;,
  1.000000;0.380122;,
  0.000000;0.400222;,
  1.000000;0.746687;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.751245;,
  1.000000;0.766033;,
  0.000000;1.000000;,
  0.000000;0.746687;,
  1.000000;0.770677;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.766033;,
  1.000000;0.751245;,
  1.000000;1.000000;,
  0.000000;0.770677;;
 }
}