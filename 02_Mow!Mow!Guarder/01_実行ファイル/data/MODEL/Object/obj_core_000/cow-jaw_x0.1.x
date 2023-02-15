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
 88;
 3.00000;0.00000;6.00000;,
 3.00000;-2.00000;3.00000;,
 3.00000;-2.00000;6.00000;,
 3.00000;0.00000;3.00000;,
 3.00000;-2.00000;0.00000;,
 3.00000;-2.00000;3.00000;,
 3.00000;0.00000;3.00000;,
 3.00000;0.00000;0.00000;,
 2.00000;0.00000;0.00000;,
 2.00000;-2.00000;-1.00000;,
 2.00000;-2.00000;0.00000;,
 2.00000;0.00000;-1.00000;,
 -2.00000;-2.00000;0.00000;,
 -2.00000;-2.00000;-1.00000;,
 -2.00000;0.00000;0.00000;,
 -2.00000;0.00000;-1.00000;,
 -3.00000;-2.00000;6.00000;,
 -3.00000;-2.00000;3.00000;,
 -3.00000;0.00000;6.00000;,
 -3.00000;-2.00000;3.00000;,
 -3.00000;-2.00000;0.00000;,
 -3.00000;0.00000;3.00000;,
 -3.00000;0.00000;3.00000;,
 -3.00000;0.00000;0.00000;,
 -0.00000;-2.00000;6.00000;,
 -0.00000;0.00000;6.00000;,
 -0.00000;0.00000;6.00000;,
 -0.00000;-2.00000;6.00000;,
 -0.00000;-2.00000;-1.00000;,
 -0.00000;0.00000;-1.00000;,
 -0.00000;-2.00000;-1.00000;,
 -0.00000;0.00000;-1.00000;,
 -0.00000;-2.00000;3.00000;,
 -0.00000;-2.00000;3.00000;,
 2.00000;0.00000;5.00000;,
 -0.00000;0.00000;5.00000;,
 -0.00000;0.00000;5.00000;,
 -2.00000;0.00000;5.00000;,
 2.00000;0.00000;5.00000;,
 -0.00000;0.00000;5.00000;,
 2.00000;0.00000;3.00000;,
 2.00000;0.00000;3.00000;,
 -2.00000;0.00000;5.00000;,
 -2.00000;0.00000;3.00000;,
 -2.00000;0.00000;3.00000;,
 2.00000;0.00000;3.00000;,
 2.00000;0.00000;1.00000;,
 2.00000;0.00000;1.00000;,
 -0.00000;0.00000;1.00000;,
 -2.00000;0.00000;3.00000;,
 -2.00000;0.00000;1.00000;,
 -2.00000;0.00000;1.00000;,
 -0.00000;0.00000;1.00000;,
 -0.00000;0.00000;1.00000;,
 3.37500;0.12500;6.43750;,
 3.37500;-2.12500;6.43750;,
 3.37500;-2.12500;3.06250;,
 3.37500;0.12500;3.06250;,
 3.37500;-2.12500;3.06250;,
 3.37500;-2.12500;-0.31250;,
 3.37500;0.12500;3.06250;,
 3.37500;0.12500;-0.31250;,
 2.25000;0.12500;-0.31250;,
 2.25000;-2.12500;-0.31250;,
 2.25000;-2.12500;-1.43750;,
 2.25000;0.12500;-1.43750;,
 -2.25000;-2.12500;-0.31250;,
 -2.25000;0.12500;-0.31250;,
 -2.25000;-2.12500;-1.43750;,
 -2.25000;0.12500;-1.43750;,
 -3.37500;-2.12500;6.43750;,
 -3.37500;0.12500;6.43750;,
 -3.37500;-2.12500;3.06250;,
 -3.37500;-2.12500;3.06250;,
 -3.37500;0.12500;3.06250;,
 -3.37500;-2.12500;-0.31250;,
 -3.37500;0.12500;3.06250;,
 -3.37500;0.12500;-0.31250;,
 -0.00000;-2.12500;6.43750;,
 -0.00000;0.12500;6.43750;,
 -0.00000;-2.12500;6.43750;,
 -0.00000;0.12500;6.43750;,
 -0.00000;-2.12500;-1.43750;,
 -0.00000;0.12500;-1.43750;,
 -0.00000;-2.12500;-1.43750;,
 -0.00000;0.12500;-1.43750;,
 -0.00000;-2.12500;3.06250;,
 -0.00000;-2.12500;3.06250;;
 
 98;
 3;2,1,0;,
 3;5,4,3;,
 3;0,1,6;,
 3;3,4,7;,
 3;10,9,8;,
 3;8,9,11;,
 3;14,13,12;,
 3;15,13,14;,
 3;18,17,16;,
 3;21,20,19;,
 3;22,17,18;,
 3;23,20,21;,
 3;2,0,24;,
 3;24,0,25;,
 3;27,26,16;,
 3;16,26,18;,
 3;10,7,4;,
 3;8,7,10;,
 3;20,14,12;,
 3;23,14,20;,
 3;28,11,9;,
 3;29,11,28;,
 3;13,31,30;,
 3;15,31,13;,
 3;2,24,1;,
 3;27,16,32;,
 3;1,24,33;,
 3;32,16,17;,
 3;5,32,4;,
 3;4,32,10;,
 3;33,19,12;,
 3;12,19,20;,
 3;10,32,9;,
 3;33,12,30;,
 3;9,32,28;,
 3;30,12,13;,
 3;34,25,0;,
 3;35,18,26;,
 3;36,25,34;,
 3;37,18,35;,
 3;6,34,0;,
 3;40,39,38;,
 3;41,34,6;,
 3;40,42,39;,
 3;43,18,37;,
 3;44,42,40;,
 3;22,18,43;,
 3;46,45,3;,
 3;47,44,40;,
 3;48,44,47;,
 3;50,21,49;,
 3;51,44,48;,
 3;7,52,46;,
 3;7,46,3;,
 3;8,52,7;,
 3;14,21,50;,
 3;14,50,53;,
 3;23,21,14;,
 3;11,52,8;,
 3;31,14,53;,
 3;29,52,11;,
 3;15,14,31;,
 3;56,55,54;,
 3;59,58,57;,
 3;56,54,60;,
 3;59,57,61;,
 3;64,63,62;,
 3;64,62,65;,
 3;68,67,66;,
 3;68,69,67;,
 3;72,71,70;,
 3;75,74,73;,
 3;72,76,71;,
 3;75,77,74;,
 3;54,55,78;,
 3;54,78,79;,
 3;81,80,70;,
 3;81,70,71;,
 3;61,63,59;,
 3;61,62,63;,
 3;67,75,66;,
 3;67,77,75;,
 3;65,82,64;,
 3;65,83,82;,
 3;85,68,84;,
 3;85,69,68;,
 3;78,55,56;,
 3;70,80,86;,
 3;78,56,87;,
 3;70,86,72;,
 3;86,58,59;,
 3;86,59,63;,
 3;73,87,66;,
 3;73,66,75;,
 3;86,63,64;,
 3;66,87,84;,
 3;86,64,82;,
 3;66,84,68;;
 
 MeshMaterialList {
  2;
  98;
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
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\Material\\mtl_pallet_000.png";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  13;
  1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;1.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.000000;1.000000;;
  98;
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;6,6,6;,
  3;6,6,6;,
  3;6,6,6;,
  3;6,6,6;,
  3;6,6,6;,
  3;6,6,6;,
  3;7,7,7;,
  3;7,7,7;,
  3;7,7,7;,
  3;7,7,7;,
  3;7,7,7;,
  3;7,7,7;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,11;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,12;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;10,10,10;,
  3;10,10,10;,
  3;10,10,10;,
  3;10,10,10;,
  3;10,10,10;,
  3;10,10,10;,
  3;10,10,10;,
  3;10,10,10;,
  3;10,10,10;,
  3;10,10,10;,
  3;10,10,10;,
  3;10,10,10;;
 }
 MeshTextureCoords {
  88;
  0.982422;0.500000;,
  0.982422;0.500000;,
  0.982422;0.500000;,
  0.798828;0.500000;,
  0.798828;0.500000;,
  0.798828;0.500000;,
  0.982422;0.500000;,
  0.798828;0.500000;,
  0.798828;0.500000;,
  0.798828;0.500000;,
  0.798828;0.500000;,
  0.798828;0.500000;,
  0.982422;0.500000;,
  0.982422;0.500000;,
  0.982422;0.500000;,
  0.982422;0.500000;,
  0.798828;0.500000;,
  0.798828;0.500000;,
  0.798828;0.500000;,
  0.982422;0.500000;,
  0.982422;0.500000;,
  0.982422;0.500000;,
  0.798828;0.500000;,
  0.982422;0.500000;,
  0.982422;0.500000;,
  0.982422;0.500000;,
  0.798828;0.500000;,
  0.798828;0.500000;,
  0.798828;0.500000;,
  0.798828;0.500000;,
  0.982422;0.500000;,
  0.982422;0.500000;,
  0.798828;0.500000;,
  0.982422;0.500000;,
  0.982422;0.500000;,
  0.798828;0.500000;,
  0.982422;0.500000;,
  0.798828;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.982422;0.500000;,
  0.896484;0.500000;,
  0.798828;0.500000;,
  0.896484;0.500000;,
  0.798828;0.500000;,
  0.798828;0.500000;,
  0.896484;0.500000;,
  0.896484;0.500000;,
  0.982422;0.500000;,
  0.982422;0.500000;,
  0.896484;0.500000;,
  0.798828;0.500000;,
  0.982422;0.500000;,
  0.982422;0.500000;,
  0.982422;0.500000;,
  0.982422;0.500000;,
  0.798828;0.500000;,
  0.798828;0.500000;,
  0.798828;0.500000;,
  0.982422;0.500000;,
  0.798828;0.500000;,
  0.798828;0.500000;,
  0.798828;0.500000;,
  0.798828;0.500000;,
  0.798828;0.500000;,
  0.982422;0.500000;,
  0.982422;0.500000;,
  0.982422;0.500000;,
  0.982422;0.500000;,
  0.798828;0.500000;,
  0.798828;0.500000;,
  0.798828;0.500000;,
  0.982422;0.500000;,
  0.982422;0.500000;,
  0.982422;0.500000;,
  0.798828;0.500000;,
  0.982422;0.500000;,
  0.982422;0.500000;,
  0.982422;0.500000;,
  0.798828;0.500000;,
  0.798828;0.500000;,
  0.798828;0.500000;,
  0.798828;0.500000;,
  0.982422;0.500000;,
  0.982422;0.500000;,
  0.798828;0.500000;,
  0.982422;0.500000;;
 }
}
