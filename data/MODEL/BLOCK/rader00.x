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
 28;
 -109.37750;418.87867;111.17471;,
 -106.89567;418.87867;-122.46763;,
 -133.82743;-0.43877;-132.74131;,
 -136.58529;-0.43877;126.89221;,
 99.50692;418.87867;-120.27521;,
 97.02511;418.87867;113.36718;,
 123.04820;-0.43877;129.65014;,
 125.80606;-0.43877;-129.98341;,
 1.59519;418.87867;112.35353;,
 -109.37750;418.87867;111.17471;,
 -136.58529;-0.43877;126.89221;,
 -65.70783;-0.43877;127.64508;,
 123.04820;-0.43877;129.65014;,
 97.02511;418.87867;113.36718;,
 3.83287;418.87867;-121.29142;,
 99.50692;418.87867;-120.27521;,
 125.80606;-0.43877;-129.98341;,
 -63.33619;-0.43877;-131.99247;,
 -133.82743;-0.43877;-132.74131;,
 -106.89567;418.87867;-122.46763;,
 3.83287;418.87867;-121.29142;,
 1.59519;418.87867;112.35353;,
 97.02511;418.87867;113.36718;,
 -106.91452;722.81974;-120.69920;,
 3.81414;722.81974;-119.52311;,
 -109.28549;722.81974;102.51397;,
 -109.37750;418.87867;111.17471;,
 1.68719;722.81974;103.69276;;
 
 12;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;8,11,12,13;,
 4;14,15,16,17;,
 4;14,17,18,19;,
 4;20,21,22,4;,
 4;23,24,20,1;,
 4;25,23,1,26;,
 4;27,25,26,21;,
 4;24,27,21,20;,
 4;24,23,25,27;;
 
 MeshMaterialList {
  3;
  12;
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
   0.501600;0.496000;0.496000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\MATERIAL\\coc_white.jpg";
   }
  }
  Material {
   0.351200;0.351200;0.351200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\MATERIAL\\conc_flore.jpg";
   }
  }
  Material {
   0.752800;0.734400;0.772000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  18;
  0.997998;0.062348;0.010601;,
  -0.999425;0.032192;-0.010616;,
  -0.997871;0.064351;-0.010600;,
  -0.010615;0.034924;0.999334;,
  -0.010614;0.038143;0.999216;,
  0.010620;0.017814;-0.999785;,
  0.010619;0.023811;-0.999660;,
  -0.999944;-0.000000;-0.010622;,
  0.010621;0.005818;-0.999927;,
  -0.010618;0.028485;0.999538;,
  -0.010616;0.033314;0.999389;,
  -0.010614;0.038143;0.999216;,
  -0.010614;0.038143;0.999216;,
  0.010618;0.023811;-0.999660;,
  0.010619;0.023811;-0.999660;,
  0.010621;0.014815;-0.999834;,
  0.000000;1.000000;0.000000;,
  0.999954;-0.000013;0.009553;;
  12;
  4;1,1,2,2;,
  4;0,0,0,0;,
  4;3,10,11,4;,
  4;3,4,12,12;,
  4;5,13,13,6;,
  4;5,6,14,15;,
  4;16,16,16,16;,
  4;8,8,5,15;,
  4;7,7,1,1;,
  4;9,9,10,3;,
  4;17,17,17,17;,
  4;16,16,16,16;;
 }
 MeshTextureCoords {
  28;
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.752650;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.727010;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.246170;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.271500;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.753830;0.000000;,
  0.752650;1.000000;,
  0.000000;1.000000;,
  1.000000;0.000000;,
  0.753830;0.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.752650;1.000000;;
 }
}
