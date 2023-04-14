// See the 'COPYING' file in the project root for licensing information.
export const Image = ({ src, alt, width, height }) => {
    return (
        <img src={src} alt={alt} width={width} height={height} />
    );
};

export default Image;
